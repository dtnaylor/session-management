#include "xwrap.h"
#include "errno.h"
#include "string.h"

/******************************************************************************
**
** FUNCTION REMAPPINGS START HERE
**
******************************************************************************/

Manager session_manager;

/*
 File I/O remappings start here
*/
int socket(int domain, int type, int protocol)
{
	int fd;
	TRACE();

	fd = __real_socket(domain, type, protocol);

	return fd;
}


int connect(int fd, const struct sockaddr *addr, socklen_t len)
{
	int rc;
	TRACE();

    //int session_interface = session_manager.getInterface(fd);
    //int session_transport = session_manager.getTransport(fd);

    // probably want to do something here to force using a 
    // specific interface + transport
	rc = __real_connect(fd, addr, len);

    printf("sending module array\n");
    // send a bit array of which modules you wish to use
    // __real_send(fd, session_manager.getModulesAsBitArray(fd),
    //             session_manager.getBitArrayLen(), 0);

    if (session_manager.instantiateModules(fd, true)) {
        return -1;
    }    
    size_t total = 4096;
    size_t used = 0;
    char *buf = (char *)malloc(total);
    EncryptionModule *enc = (EncryptionModule *)session_manager.getFront(fd);
    while(!enc->handshake_done()) {
        enc->data_out(buf, &used, &total);
        __real_send(fd, buf, used, 0);
        if(enc->handshake_done()) break;
        used = __real_recv(fd, buf, total, 0);
        enc->data_in(buf, &used, &total);
        used = 0;
    }
    free(buf);
    printf("completely done with handshake\n");

	return rc;
}

int bind(int fd, const struct sockaddr *addr, socklen_t len)
{
	int rc;
	TRACE();

	rc = __real_bind(fd, addr, len);

	return rc;
}

int accept(int fd, struct sockaddr *addr, socklen_t *addr_len)
{
	int new_fd;
	TRACE();

	new_fd = __real_accept(fd, addr, addr_len);

    if (session_manager.instantiateModules(new_fd, false)) {
        return -1;
    }

    size_t total = 4096;
    size_t used = 0;
    char *buf = (char *)malloc(total);
    EncryptionModule *enc = (EncryptionModule *)session_manager.getFront(new_fd);
    printf("starting handshake\n");
    while(!enc->handshake_done()) {
        used = __real_recv(new_fd, buf, total, 0);
        enc->data_in(buf, &used, &total);
        if(enc->handshake_done()) break;
        used = 0;
        enc->data_out(buf, &used, &total);
        __real_send(new_fd, buf, used, 0);
    }
    free(buf);
    printf("completely done with handshake\n");

    // char lenc[1];
    // if (__real_recv(fd, lenc, 1, 0) == -1) {
        
    //     printf("bad len\n");
    //     return -1;
    // }
    // printf("got len\n");
    // int len = atoi(lenc);
    // char *buf = (char *) malloc(len);
    // if (__real_recv(fd, buf, len, 0) == -1) {
    //     return -1;
    // }
    // printf("got module buffer\n");
    // if (session_manager.instantiateModulesFromBitArray(fd, buf, len)) {
    //     return -1;
    // }

	return new_fd;
}


ssize_t send(int fd, const void *buf, size_t n, int flags)
{
	int rc;
    size_t original_size = n;
	TRACE();

    size_t buflen = 4096;
    char *new_buf = (char*)malloc(buflen);
    memcpy(new_buf, buf, n);
    if(session_manager.send(fd, new_buf, &n, &buflen) <= 0) {
        return -1;
    }
	rc = __real_send(fd, new_buf, n, flags);
    free(new_buf);

    if(rc <= 0) return rc; // HACK!!!
	return original_size;
}

ssize_t recv(int fd, void *buf, size_t n, int flags)
{
	size_t rc;
	TRACE();

    size_t buflen = 4096;
    char *new_buf = (char*)malloc(buflen);
	rc = __real_recv(fd, new_buf, buflen, flags);
    if (rc > 0) {
        // Need to be careful that we don't exceed buflen
        session_manager.recv(fd, new_buf, &rc, &buflen);
    }
    memcpy(buf, new_buf, n);
    free(new_buf);

    ssize_t rc2 = (ssize_t)rc;
    if(rc > n) return n; // HACK!!!
	return rc2;
}

ssize_t sendto(int fd, const void *buf, size_t n, int flags, const struct sockaddr *addr, socklen_t addr_len)
{
	int rc;
	TRACE();

	rc = __real_sendto(fd, buf, n, flags, addr, addr_len);

	return rc;
}

ssize_t recvfrom(int fd, void *buf, size_t n, int flags, struct sockaddr *addr, socklen_t *addr_len)
{
	int rc;
	TRACE();

	rc = __real_recvfrom(fd, buf, n, flags, addr, addr_len);

	return rc;
}

int listen(int fd, int n)
{
	TRACE();
	return __real_listen(fd, n);
}

int close(int fd)
{
	int rc;
	TRACE();

	rc = __real_close(fd);
    session_manager.close(fd);

	return rc;
}
