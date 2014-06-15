/*
** Copyright 2014 Carnegie Mellon University
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**    http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include <string.h>
#include "EncryptionModule.h"

/* Constructor */
EncryptionModule::EncryptionModule() {
	handshake_done_ = false;
	ready_ = false;
}

/* Destructor */
EncryptionModule::~EncryptionModule() {
	RSA_free(master_keypair_);
	RSA_free(session_keypair_);
	if (en_) EVP_CIPHER_CTX_cleanup(en_);
	if (de_) EVP_CIPHER_CTX_cleanup(de_);
}
		
void EncryptionModule::initialize(Preferences &prefs, ContextualInfo &info, bool initiator) {
	// do nothing
	(void)prefs;
	(void)info;
	(void)initiator;

    printf("intializing encryption\n");

	// if we're the initiator, first step in handshake is to send client hello.
	// if we're the server, first step is to wait for client hello
	handshake_status_ = initiator ? kSendClientHello : kWaitForClientHello;

	// generate a new master keypair (on a real server, this would be long-lived
	// and signed by a CA)
	master_keypair_ = RSA_generate_key(RSA_KEY_LENGTH, RSA_PUB_EXPONENT, NULL, NULL);

	// generate a single-use keypair for this session
	session_keypair_ = RSA_generate_key(RSA_KEY_LENGTH, RSA_PUB_EXPONENT, NULL, NULL);

	// allocate and initialize state for the symmetric cipher
	en_ = (EVP_CIPHER_CTX*)malloc(sizeof(EVP_CIPHER_CTX));
	EVP_CIPHER_CTX_init(en_);
	de_ = (EVP_CIPHER_CTX*)malloc(sizeof(EVP_CIPHER_CTX));
	EVP_CIPHER_CTX_init(de_);
    printf("done initalize\n");
}

bool EncryptionModule::handshake_done() {
	return handshake_done_;
}

bool EncryptionModule::ready() {
	return ready_;
}

DataPathState EncryptionModule::get_required_data_path_state() {
	return kAny;
}

DataPathState EncryptionModule::get_resulting_data_path_state() {
	return kObscured;
}

int EncryptionModule::data_in(char *buf, size_t *datalen, size_t *buflen) {
	if (!handshake_done_) {
		return handshake_in(buf, datalen, buflen);
	}

	/* to decrease the chance we don't receive the full message,
	   don't process unless we're on an AES_BLOCK boundary.
	   FIXME: still possible we don't get all blocks; to fix this we
	   really need an SSL header w/ message size up front */
	if (*datalen % AES_BLOCK_SIZE != 0) {
		WARN("data_in can only process on AES block boundaries; waiting for more data\n");
		return ERR_NEED_MORE_DATA;
	}
	/* alloc a temp buffer to hold the ciphertext */
	char* ciphertext = (char *)malloc(*buflen);
    memcpy(ciphertext, buf, *datalen);
	/* decrpyt the ciphertext */
	int plaintext_len = aes_decrypt(de_,
							(unsigned char*)buf, *buflen,
							(unsigned char*)ciphertext, *datalen);
	if (plaintext_len <= 0) {
		ERROR("ERROR decrypting message");
		return ERR_GENERIC;
	}
	
	/* free the temp buffer */
	free(ciphertext);

	/* update sizes */
	*datalen = plaintext_len;

	return 0;
}

int EncryptionModule::data_out(char *buf, size_t *datalen, size_t *buflen) {
	
	if (!handshake_done_) {
		return handshake_out(buf, datalen, buflen);
	}

	/* alloc a new buffer to hold the ciphertext */
    /* alloc a temp buffer to hold the plaintext */
    char *plaintext = (char*)malloc(*buflen);
    memcpy(plaintext, buf, *datalen);

	/* encrypt data to send */
	int ciphertext_len = aes_encrypt(en_, 
							(unsigned char*)plaintext, *datalen,
							(unsigned char*)buf, *buflen);
	if (ciphertext_len <= 0) {
		ERROR("ERROR encrypting message");
		return ERR_GENERIC;
	}

	/* free the temporary buf */
	free(plaintext);

	/* update sizes */
	*datalen = ciphertext_len;

	return 0;
}








/* handshake */

int EncryptionModule::handshake_in(char *buf, size_t *datalen, size_t *buflen) {
    (void)buflen;

	/* pick up where we left off in the handshake */
	switch(handshake_status_) {
		case kWaitForClientHello: // server-side
		{
			/* make sure the data is a complete client hello */
			if (strncmp("CLIENT HELLO", buf, 12) != 0) {
				ERROR("ERROR: received message was not CLIENT HELLO\n");
				return ERR_NEED_MORE_DATA;
			} else {
				// FIXME: inefficient!
				// int hello_len = strlen("CLIENT HELLO");
				// char* newbuf = (char*)malloc(*buflen);
				// memcpy(newbuf, buf+hello_len, *datalen-hello_len);
				// *datalen -= hello_len;
				// free(buf);
				// buf = newbuf;  // FIXME: need pointer to pointer

				handshake_status_ = kSendServerHello;
				return 0;
			}
			
		}


		case kWaitForServerHello:  // client-side
		{
			/* Wait for SERVER HELLO */
			// Receive public key + temporary public key signed by server's long-term private key
			// Continue receiving until we see "SERVER DONE"
			
			// FIXME: this assumes there's no handshake data after us!
			if (*datalen < 11 || strcmp("SERVER DONE", (buf+*datalen-11)) != 0) {
				ERROR("Did not receive complete SERVER DONE. Waiting for more data\n");
				return ERR_NEED_MORE_DATA;
			}

			/* Parse public keys from SERVER HELLO */
			int offset = strlen("SERVER HELLO");
			char *keys = buf+offset;  // start of signed portion of message

			uint32_t *keybufsizeptr = (uint32_t*)(buf+offset);
			uint32_t keybufsize = *keybufsizeptr; // TODO: error checking here
			offset += sizeof(uint32_t);
			RSA *pubkey = deserialize_rsa_pub_key(buf+offset, keybufsize);
			offset += keybufsize;

			uint32_t *tempkeybufsizeptr = (uint32_t*)(buf+offset);
			uint32_t tempkeybufsize = *tempkeybufsizeptr;
			offset += sizeof(uint32_t);
			session_pub_key_ = deserialize_rsa_pub_key(buf+offset, tempkeybufsize);
			offset += tempkeybufsize;

			uint32_t *siglenptr = (uint32_t*)(buf+offset);
			uint32_t siglen = *siglenptr;
			offset += sizeof(uint32_t);
			char* sig = buf+offset;
			offset += siglen;
			DBGF("Received keys:\n\tkeylen: %d\n\ttempkeylen: %d\n\tsiglen: %d", keybufsize, tempkeybufsize, siglen);

			/* Verify two things:
			 *	1) in the real world, verify the key in the cert
			 *	2) verify the sig so we trust the temp key
			 */
			if (verify(pubkey, keys, keybufsize+tempkeybufsize+2*sizeof(uint32_t), sig, siglen) != 1) {
				WARN("Unable to verify signature on temporary RSA keypair! Closing connection.");
				return ERR_GENERIC;
			}

			// FIXME: inefficient!
			// char* newbuf = (char*)malloc(*buflen);
			// int used_len = offset + strlen("SERVER DONE");
			// memcpy(newbuf, buf+used_len, *datalen-used_len);
			// *datalen -= used_len;
			// free(buf);
			// buf = newbuf; // FIXME need pointer to pointer

			handshake_status_ = kSendPreMasterSecret;
			return 0;
		}


		case kWaitForPreMasterSecret:  // server-side
		{
			/* Receive and decrypt pre-master secret */
			size_t expecting = RSA_size(session_keypair_); // TODO: have client send key size?
			if (*datalen < expecting) {
				WARN("Did not receive full PMS. Waiting for more data.\n");
				return ERR_NEED_MORE_DATA;
			}


			unsigned char* pms = (unsigned char*)malloc(RSA_size(session_keypair_));
			int plaintext_len;
			if ( (plaintext_len = priv_decrypt(session_keypair_, 
									pms, RSA_size(session_keypair_), 
									(unsigned char*)buf, *datalen)) == -1) {  // FIXME what if there's more data for modules above us?
				ERROR("ERROR decrypting session key");
				return ERR_GENERIC;
			}
			if (plaintext_len != PRE_MASTER_SECRET_LENGTH) {
				ERROR("Decrypted key material is not of size PRE_MASTER_SECRET_LENGTH");
				return ERR_GENERIC;
			}
			if (VERBOSITY >= V_DEBUG) {
				DBG("Pre_Master Secret:");
				print_bytes(pms, PRE_MASTER_SECRET_LENGTH);
			}


			/* Init symmetric session ciphers with pre-master secret.
			   Client encrypt context initialized with same key data as
			   server decrypt context and vice versa. */
			uint32_t salt[] = SALT;
			if (aes_init(&pms[PRE_MASTER_SECRET_LENGTH/2], PRE_MASTER_SECRET_LENGTH/2, 
						 pms, PRE_MASTER_SECRET_LENGTH/2, 
						 (unsigned char *)&salt, en_, de_)) {
				ERROR("ERROR initializing AES ciphers");
				return ERR_GENERIC;
			}
			free(pms);
			

			/* remove our handshake data before next module gets buf */
			// FIXME: inefficient!
			// char* newbuf = (char*)malloc(*buflen);
			// memcpy(newbuf, buf+expecting, *datalen-expecting);
			// *datalen -= expecting;
			// free(buf);
			// buf = newbuf; // FIXME need pointer to pointer


			/* done with handshake */
			handshake_status_ = kDone;
			handshake_done_ = true;
			ready_ = true;
			return 0;
		}
		default:
			ERROR("Unknown handshake status\n");
            printf("handshake status: %d\n", handshake_status_);
			return ERR_GENERIC;
	}

}

int EncryptionModule::handshake_out(char *buf, size_t *datalen, size_t *buflen) {

	/* pick up where we left off in the handshake.
	   append the next portion of the handshake to buf, after whatever handshake
	   data may be ther already. */
	switch(handshake_status_) {
		case kSendClientHello:  // client-side
		{
			/* Send CLIENT HELLO */
			int hello_len = sprintf((buf+*datalen), "CLIENT HELLO");
			*datalen += hello_len;

			handshake_status_ = kWaitForServerHello;
			return 0;
		}
		case kSendServerHello:  // server-side
		{
			/* Send SERVER HELLO */
			int hello_len = sprintf((buf+*datalen), "SERVER HELLO");
			*datalen += hello_len;

			/* Send public key + temporary public key signed by long-term private key */
			//
			// Copy the following into buf:
			// keylen || key || tempkeylen || tempkey || siglen || sig
			//
			size_t offset = 0;
			char* base = buf+*datalen;

            
			int base_buflen = *buflen-*datalen;

			uint32_t keybufsize = serialize_rsa_pub_key(master_keypair_, base+sizeof(uint32_t), base_buflen-offset-sizeof(uint32_t));
			uint32_t* keybufsizeptr = (uint32_t*)base;
			*keybufsizeptr = keybufsize;
			offset += (sizeof(uint32_t) + keybufsize);

			uint32_t tempkeybufsize = serialize_rsa_pub_key(session_keypair_, base+sizeof(uint32_t)+offset, base_buflen-offset-sizeof(uint32_t));
			uint32_t* tempkeybufsizeptr = (uint32_t*)(base+offset);
			*tempkeybufsizeptr = tempkeybufsize;
			offset += (sizeof(uint32_t) + tempkeybufsize);

			uint32_t siglen = sign(master_keypair_, base, offset, base+offset+sizeof(uint32_t), base_buflen-offset-sizeof(uint32_t));
			uint32_t* siglenptr = (uint32_t*)(base+offset);
			*siglenptr = siglen;
			offset += sizeof(uint32_t);
			offset += siglen;

			*datalen += offset;

			DBGF("Sent keys (%lu bytes)\n\tkeylen: %d\n\ttempkeylen: %d\n\tsiglen:%d", offset, keybufsize, tempkeybufsize, siglen);


			/* Send SERVER (hello) DONE */
			int done_len = sprintf((buf+*datalen), "SERVER DONE");
			*datalen += done_len;

			handshake_status_ = kWaitForPreMasterSecret;
			return 0;
		}


		case kSendPreMasterSecret:  // client-side
		{
			// We can start adding data at base
			char* base = buf+*datalen;
			int base_buflen = *buflen-*datalen;

			/* Generate pre-master secret and send to server, encrypted with session_pub_key_ */
			unsigned char* pms = (unsigned char*)malloc(PRE_MASTER_SECRET_LENGTH);
    		if (RAND_bytes(pms, PRE_MASTER_SECRET_LENGTH) != 1) {
    		    ERROR("ERROR: Couldn't generate pre-master secret");
				return ERR_GENERIC;
    		}
			if (VERBOSITY >= V_DEBUG) {
				DBG("Pre-Master Secret:");
				print_bytes(pms, PRE_MASTER_SECRET_LENGTH);
			}

			int ciphertext_len;
			if ( (ciphertext_len = pub_encrypt(session_pub_key_, 
									pms, PRE_MASTER_SECRET_LENGTH, 
									base, base_buflen)) == -1 ) {
				ERROR("ERROR: Unable to encrypt session key");
				return ERR_GENERIC;
			}

			*datalen += ciphertext_len;
    		


			/* Init symmetric session ciphers with pre-master secret.
			   Client encrypt context initialized with same key data as
			   server decrypt context and vice versa. */
			uint32_t salt[] = SALT;
			if (aes_init(pms, PRE_MASTER_SECRET_LENGTH/2, 
						 &pms[PRE_MASTER_SECRET_LENGTH/2], PRE_MASTER_SECRET_LENGTH/2, 
						 (unsigned char *)&salt, en_, de_)) {
				ERROR("ERROR initializing AES ciphers");
				return ERR_GENERIC;
			}
			free(pms);


			/* done with handshake */
			handshake_status_ = kDone;
			handshake_done_ = true;
			ready_ = true;
			return 0;
		}
		default:
			ERROR("Unknown handshake status\n");
            printf("handshake status: %d\n", handshake_status_);
			return ERR_GENERIC;
	}
}





/* below is a modified version of XSSL_util.c */

/**
* @brief Prints an RSA keypair.
*
* @param keypair
*/
void print_keypair(RSA *keypair) {
    // To get the C-string PEM form:
    BIO *pri = BIO_new(BIO_s_mem());
    BIO *pub = BIO_new(BIO_s_mem());
    
    PEM_write_bio_RSAPrivateKey(pri, keypair, NULL, NULL, 0, NULL, NULL);
    PEM_write_bio_RSAPublicKey(pub, keypair);
    
    size_t pri_len = BIO_pending(pri);
    size_t pub_len = BIO_pending(pub);
    
	char *pri_key, *pub_key;
    pri_key = (char *)malloc(pri_len + 1);
    pub_key = (char *)malloc(pub_len + 1);
    
    BIO_read(pri, pri_key, pri_len);
    BIO_read(pub, pub_key, pub_len);
    
    pri_key[pri_len] = '\0';
    pub_key[pub_len] = '\0';
    
    printf("\n%s\n\n%s\n", pri_key, pub_key);

    BIO_free_all(pub);
    BIO_free_all(pri);
    free(pri_key);
    free(pub_key);
}

/**
* @brief Print bytes in hex.
*
* @param bytes
* @param len
*/
void print_bytes(unsigned char *bytes, size_t len) {
	
	// format key as hex string
	char str[len*2 + 1];
	for (size_t i = 0; i < len; i++) {
		sprintf(&str[i*2], "%02x", bytes[i]);
	}
    str[len*2] = '\0';
	
	printf("%s\n\n", str);
}


/**
* @brief Serialize the supplied RSA public key to the supplied buffer.
*
* Stores the RSA public key in the supplied buffer. If the space required is
*	larger than len (the size of the buffer), the only the first len bytes will
*	be placed in the buffer. Check that the return value is <= len.
*
* @param keypair The keypair to serialize
* @param buf Buffer for the result.
* @param len Size of the buffer.
*
* @return The size of the serialized keypair. This may be bigger than len,
*	indicating the result has been truncated and should not be used.
*/
uint32_t serialize_rsa_pub_key(RSA *keypair, char *buf, size_t len) {

	size_t bufSpaceLeft = len;

    char *hexformod = BN_bn2hex(keypair->n);
	uint32_t hexformodsize = strlen(hexformod);
    char *hexforexp = BN_bn2hex(keypair->e);
	uint32_t hexforexpsize = strlen(hexforexp);
	DBGF("hexformod: %s (%d)", hexformod, hexformodsize);
	DBGF("hexforexp: %s (%d)", hexforexp, hexforexpsize);

	if (bufSpaceLeft > sizeof(uint32_t)) {
		uint32_t *modsizeptr = (uint32_t*)buf;
		*modsizeptr = hexformodsize;
		bufSpaceLeft -= sizeof(uint32_t);
	}
	if (bufSpaceLeft > hexformodsize) {
		memcpy(&buf[sizeof(uint32_t)], hexformod, hexformodsize);
		bufSpaceLeft -= hexformodsize;
	}
	if (bufSpaceLeft > sizeof(uint32_t)) {
		uint32_t *expsizeptr = (uint32_t*)&buf[sizeof(uint32_t) + hexformodsize];
		*expsizeptr = hexforexpsize;
		bufSpaceLeft -= sizeof(uint32_t);
	}
	if (bufSpaceLeft > hexforexpsize) {
		memcpy(&buf[2*sizeof(uint32_t) + hexformodsize], hexforexp, hexforexpsize);
		bufSpaceLeft -= hexforexpsize;
	}

	uint32_t total_size = strlen(hexformod) + strlen(hexforexp) + 2*sizeof(uint32_t);
	free(hexformod);
	free(hexforexp);

	return total_size;
}

/**
* @brief Deserialize the supplied buffer into an RSA public key.
*
* Allocate a new RSA keypair and fill in the modulus and the exponent from
*	the public key stored in buf.
*
* @param buf Buffer containing the key to deserialize.
* @param len Length of the buffer.
*
* @return RSA *keypair on success, NULL on failure
*/
RSA *deserialize_rsa_pub_key(char *buf, size_t len) {
	(void)len; // TODO: use len to make sure we don't try to read past end of buffer

	/* get modulus */
	uint32_t hexformodsize = *(uint32_t*)buf;
	char *hexformod = (char*)malloc(hexformodsize + 1);
	memcpy(hexformod, &buf[sizeof(uint32_t)], hexformodsize);
	hexformod[hexformodsize] = '\0';
	DBGF("hexformod: %s", hexformod);

	/* get exponent */
	uint32_t hexforexpsize = *(uint32_t*)&buf[sizeof(uint32_t) + hexformodsize];
	char *hexforexp = (char*)malloc(hexforexpsize + 1);
	memcpy(hexforexp, &buf[2*sizeof(uint32_t) + hexformodsize], hexforexpsize);
	hexforexp[hexforexpsize] = '\0';
	DBGF("hexforexp: %s", hexforexp);

	/* make the keypair */
    RSA *keypair = RSA_new();
    BN_hex2bn(&keypair->n, hexformod); // TODO: need BN_new() first?
    BN_hex2bn(&keypair->e, hexforexp);
    keypair->d = NULL;
    keypair->p = NULL;
    keypair->q = NULL;
    keypair->dmp1 = NULL;
    keypair->dmq1 = NULL;
    keypair->iqmp = NULL;

	free(hexformod);
	free(hexforexp);

	return keypair;
}

/**
* @brief Hash the supplied message (SHA-1)
*
* @param msg Message to hash.
* @param msg_len Size of message to hash.
*
* @return Pointer to buffer containing hash on success
* 	NULL on failure
*/
unsigned char *myhash(char *msg, size_t msg_len) {
	SHA_CTX ctx;
	unsigned char *digest = (unsigned char*)malloc(SHA_DIGEST_LENGTH);
	if (SHA1_Init(&ctx) == 0) {
		ERROR("ERROR initing SHA1 context");
		return NULL;
	}
	if (SHA1_Update(&ctx, msg, msg_len) == 0) {
		ERROR("ERROR updating SHA1 context");
		return NULL;
	}
	if (SHA1_Final(digest, &ctx) == 0) {
		ERROR("ERROR computing digest");
		return NULL;
	}

	return digest;
}

/**
* @brief Sign a message with provided RSA key.
*
* Hash msg (SHA1) and encrypt the resulting digest. The buffer supplied to hold
* the signature must be at least RSA_size(keypair) bytes long.
*
* @param msg Data to sign.
* @param msg_len Size of data to sign.
* @param sigbuf Buffer to hold created signature.
* @param sigbuflen Space available for signature.
*
* @return Size of signature on success, 0 on error.
*/
uint32_t sign(RSA *keypair, char *msg, size_t msg_len, char *sigbuf, int sigbuflen) {
	
	if (sigbuflen < RSA_size(keypair)) {
		ERROR("ERROR: Could not sign message because sigbuf is too small");
		return 0;
	}

	/* first hash msg */
	unsigned char *digest = myhash(msg, msg_len);
	if (digest == NULL) {
		ERROR("ERROR: Unable to hash message");
		return 0;
	}

	/* now sign the hash */
    uint32_t siglen;
    if (RSA_sign(NID_sha1, digest, SHA_DIGEST_LENGTH, (unsigned char*)sigbuf, &siglen, keypair) != 1) {

    	char *err = (char *)malloc(130); //FIXME?
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), err);
        ERRORF("Error signing message: %s", err);
        
        free(err);
        return 0;
    }

	free(digest);
	return siglen;
}

/**
* @brief Verify a signature with the provided RSA public key.
*
* @param keypair Public key for verifying signature.
* @param msg Message to verify.
* @param msg_len Size of message to verify.
* @param sig Signature to verify.
* @param sig_len Size of signature to verify.
*
* @return 1 on successful verification, 0 otherwise
*/
int verify(RSA *keypair, char *msg, size_t msg_len, char *sig, size_t sig_len) {

	/* first hash msg */
	unsigned char *digest = myhash(msg, msg_len);
	if (digest == NULL) {
		ERROR("ERROR: Unable to hash message");
		return 0;
	}

	/* now verify signature */
	int rc = RSA_verify(NID_sha1, digest, SHA_DIGEST_LENGTH, (unsigned char*)sig, sig_len, keypair);

	free(digest);
	return rc;
}

/**
* @brief Encrypt a message using the provided RSA public key.
*
* @param keypair
* @param msg Message to encrypt.
* @param msg_len Size of message to encrypt.
* @param ciphertext_buf Buffer to be filled with ciphertext.
* @param ciphertext_buf_len Size of ciphertext buffer. Should be >= RSA_size(keypair).
*
* @return Size of encrypted message on success, -1 on failure.
*/
int pub_encrypt(RSA *keypair, unsigned char *msg, size_t msg_len, char *ciphertext_buf, int ciphertext_buf_len) {
	if (ciphertext_buf_len < RSA_size(keypair)) {
		ERROR("ERROR: Not enough room for ciphertext");
		return -1;
	}

    int ciphertext_len;
    if((ciphertext_len = RSA_public_encrypt(msg_len, msg, (unsigned char*)ciphertext_buf, keypair, RSA_PKCS1_OAEP_PADDING)) == -1) {
    	char *err = (char *)malloc(130); //FIXME?
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), err);
        ERRORF("Error encrypting message: %s", err);
        
        free(err);
        return -1;
    }
    
	return ciphertext_len;
}

/**
* @brief Decrpyt a message using the provided RSA private key.
*
* @param keypair
* @param msg_buf Empty buffer for decrypted message.
* @param msg_buf_len Size of message buffer. Should be >= RSA_size(keypair).
* @param ciphertext Ciphertext to decrypt.
* @param ciphertext_len Size of ciphertext.
*
* @return Size of decrypted message on success, -1 on failure.
*/
int priv_decrypt(RSA *keypair, unsigned char *msg_buf, int msg_buf_len, unsigned char *ciphertext, size_t ciphertext_len) {
	if (msg_buf_len < RSA_size(keypair)) {
		ERROR("ERROR: Not enough room for plaintext");
		return -1;
	}

	int plaintext_len;
    if( (plaintext_len = RSA_private_decrypt(ciphertext_len, ciphertext, msg_buf, keypair, RSA_PKCS1_OAEP_PADDING)) == -1) {
    	char *err = (char *)malloc(130); //FIXME?
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), err);
        ERRORF("error decrypting message: %s", err);
        
        free(err);
		return -1;
    }

	return plaintext_len;
}


/**
 **/
/**
* @brief Prepares AES encryption and decryption contexts.
*
* Create an 256 bit key and IV using the supplied key_data. salt can be added for taste.
* Fills in the encryption and decryption ctx objects and returns 0 on success
*
* Code adapted from: http://saju.net.in/code/misc/openssl_aes.c.txt
*
* @param key_data
* @param key_data_len
* @param salt
* @param e_ctx
* @param d_ctx
*
* @return 0 on success
*/
int aes_init(unsigned char *en_key_data, int en_key_data_len, 
			 unsigned char *de_key_data, int de_key_data_len, 
			 unsigned char *salt, EVP_CIPHER_CTX *e_ctx, 
             EVP_CIPHER_CTX *d_ctx) {
	int i, nrounds = 5;
	unsigned char en_key[32], en_iv[32], de_key[32], de_iv[32];
	
	/*
	 * Gen key & IV for AES 256 CBC mode. A SHA1 digest is used to hash the supplied key material.
	 * nrounds is the number of times the we hash the material. More rounds are more secure but
	 * slower.
	 */
	i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), salt, en_key_data, en_key_data_len, nrounds, en_key, en_iv);
	if (i != 32) {
	  ERRORF("Key size is %d bits - should be 256 bits", i);
	  return -1;
	}
	if (EVP_EncryptInit_ex(e_ctx, EVP_aes_256_cbc(), NULL, en_key, en_iv) != 1) {
		ERROR("ERROR initializing encryption context");
		return -1;
	}

	i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), salt, de_key_data, de_key_data_len, nrounds, de_key, de_iv);
	if (i != 32) {
	  ERRORF("Key size is %d bits - should be 256 bits", i);
	  return -1;
	}
	if (EVP_DecryptInit_ex(d_ctx, EVP_aes_256_cbc(), NULL, de_key, de_iv) != 1) {
		ERROR("ERROR initializing decryption context");
		return -1;
	}
	
	return 0;
}


/**
* @brief Encrypt *len bytes of data
*
* All data going in & out is considered binary (unsigned char[])
*
* Code adapted from: http://saju.net.in/code/misc/openssl_aes.c.txt
*
* @param e
* @param plaintext
* @param plaintext_len
* @param ciphertext_buf
* @param ciphertext_buf_len
*
* @return Size of ciphertext on success, -1 on failure
*/
int aes_encrypt(EVP_CIPHER_CTX *e, 
				unsigned char *plaintext, int plaintext_len,
				unsigned char *ciphertext_buf, int ciphertext_buf_len) {
	/* max ciphertext len for a n bytes of plaintext is n + AES_BLOCK_SIZE -1 bytes */
	int c_len = plaintext_len + AES_BLOCK_SIZE, f_len = 0;
	if (ciphertext_buf_len < c_len) {
		ERROR("Ciphertext buffer too small");
		return -1;
	}
	
	/* allows reusing of 'e' for multiple encryption cycles */
	if (EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL) != 1) {
		ERROR("ERROR initializing encryption");
		return -1;
	}
	
	/* update ciphertext, c_len is filled with the length of ciphertext generated,
	  *len is the size of plaintext in bytes */
	if (EVP_EncryptUpdate(e, ciphertext_buf, &c_len, plaintext, plaintext_len) != 1) {
		ERROR("ERROR encrypting");
		return -1;
	}
	
	/* update ciphertext with the final remaining bytes */
	if (EVP_EncryptFinal_ex(e, ciphertext_buf+c_len, &f_len) != 1) {
		ERROR("ERROR encrypting");
		return -1;
	}
	
	return c_len + f_len;
}


/**
* @brief Decrypt *len bytes of ciphertext.
*
* Code adapted from: http://saju.net.in/code/misc/openssl_aes.c.txt
*
* @param e
* @param plaintext_buf
* @param plaintext_buf_len
* @param ciphertext
* @param ciphertext_len
*
* @return Size of decrypted data on success; -1 on failure
*/
int aes_decrypt(EVP_CIPHER_CTX *e, 
				unsigned char *plaintext_buf, int plaintext_buf_len,
				unsigned char *ciphertext, int ciphertext_len) {
	/* because we have padding ON, we must have an extra cipher block size of memory */
	int p_len = ciphertext_len, f_len = 0;
	if (plaintext_buf_len < p_len + AES_BLOCK_SIZE) {
		ERROR("ERROR: plaintext buffer too small");
		return -1;
	}
	
	if (EVP_DecryptInit_ex(e, NULL, NULL, NULL, NULL) != 1) {
		ERROR("ERROR initializing decryption");
		return -1;
	}
	if (EVP_DecryptUpdate(e, plaintext_buf, &p_len, ciphertext, ciphertext_len) != 1) {
		ERROR("ERROR decrypting");
		return-1;
	}
	if (EVP_DecryptFinal_ex(e, plaintext_buf+p_len, &f_len) != 1) {
		ERROR("ERROR decrypting");
		return -1;
	}
	
	return p_len + f_len;;
}
