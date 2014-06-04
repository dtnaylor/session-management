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

#ifndef ENCRYPTION_MODULE_H
#define ENCRYPTION_MODULE_H

#include <stdint.h>

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/evp.h>

#include "DataPathModule.h"


#define RSA_KEY_LENGTH 1024  /* in bits */
#define RSA_PUB_EXPONENT 3
#define PRE_MASTER_SECRET_LENGTH 32  /* in bytes */
#define SALT {7509, 9022}

/* constants for tracking handshake progress */
enum HandshakeStatus {
	kSendClientHello,
	kWaitForServerHello,
	kSendPreMasterSecret,

	kWaitForClientHello,
	kSendServerHello,
	kWaitForPreMasterSecret,

	kDone
};

class EncryptionModule : public DataPathModule {
	public:
		virtual void initialize(Preferences &prefs, ContextualInfo &info, bool initiator);
		virtual bool handshake_done();
		virtual bool ready();
		virtual DataPathState get_required_data_path_state();
		virtual DataPathState get_resulting_data_path_state();
		virtual int data_in(void *buf, size_t *datalen, size_t *buflen);
		virtual int data_out(void *buf, size_t *datalen, size_t *buflen);

		EncryptionModule();
		~EncryptionModule();

	private:
		bool handshake_done_;
		bool ready_;

		/* handshake helper methods */
		int handshake_in(void *buf, size_t *datalen, size_t *buflen);
		int handshake_out(void *buf, size_t *datalen, size_t *buflen);

		/* keep track of handshake progress */
		HandshakeStatus handshake_status_;
		RSA *session_pub_key_;

		RSA *master_keypair_;
		RSA *session_keypair_;
		EVP_CIPHER_CTX *en_;  // encryption context
		EVP_CIPHER_CTX *de_;  // decryption context
};





/* Below is a modified version of xssl.h */

//typedef struct {
//	RSA *keypair;
//	
//} XSSL_CTX;
//
//
//typedef struct {
//	XSSL_CTX *ctx;
//	RSA *session_keypair;
//	int sockfd;
//	EVP_CIPHER_CTX *en;  // encryption context
//	EVP_CIPHER_CTX *de;  // decryption context
//} XSSL;


/* SSL Functions */
//XSSL_CTX *XSSL_CTX_new();
//void XSSL_CTX_free(XSSL_CTX *ctx);
//
//XSSL *XSSL_new(XSSL_CTX *ctx);
//void XSSL_free(XSSL *xssl);
//
//int XSSL_set_fd(XSSL *xssl, int fd);
//int XSSL_accept(XSSL *xssl);
//int XSSL_connect(XSSL *xssl);
//int XSSL_write(XSSL *xssl, const void *buf, int num);
//int XSSL_read(XSSL *xssl, void *buf, int num);
//int XSSL_shutdown(XSSL *xssl);
//
//int XSSL_encrypt(XSSL *xssl, const void *plaintext, int len, void *ciphertext, int size);
//int XSSL_decrypt(XSSL *xssl, const void *ciphertext, int len, void *plaintext, int size);
//int XSSL_connect_ip(XSSL *xssl);
//int XSSL_accept_ip(XSSL *xssl);


/* Utility Functions */
char* SID_from_keypair(RSA *keypair);
void print_keypair(RSA *keypair);
void print_bytes(unsigned char *bytes, size_t len);
uint32_t serialize_rsa_pub_key(RSA *keypair, char *buf, size_t len);
RSA *deserialize_rsa_pub_key(char *buf, size_t len);
unsigned char *hash(char *msg, size_t msg_len);
uint32_t sign(RSA *keypair, char *msg, size_t msg_len, char *sigbuf, int sigbuflen);
int verify(RSA *keypair, char *msg, size_t msg_len, char *sig, size_t sig_len);
int pub_encrypt(RSA *keypair, unsigned char *msg, size_t msg_len, char *ciphertext_buf, int ciphertext_buf_len);
int priv_decrypt(RSA *keypair, unsigned char *msg_buf, int msg_buf_len, unsigned char *ciphertext, size_t ciphertext_len);
int aes_init(unsigned char *en_key_data, int en_key_data_len, unsigned char *de_key_data, int de_key_data_len, unsigned char *salt, EVP_CIPHER_CTX *e_ctx, EVP_CIPHER_CTX *d_ctx);
int aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int plaintext_len, unsigned char *ciphertext_buf, int ciphertext_buf_len);
int aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext_buf, int plaintext_buf_len, unsigned char *ciphertext, int ciphertext_len);




#endif /* ENCRYPTION_MODULE_H */
