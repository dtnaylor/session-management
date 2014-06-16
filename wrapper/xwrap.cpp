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
    //return rc;
    if(rc == -1) {
        __real_fprintf(_log, "errno: %s\n", strerror(errno));
        rc = 0;
        //return rc; 
    }
    __real_fprintf(_log, "connects fd %d, rc %d\n", fd, rc);


    //printf("sending module array\n");
    // send a bit array of which modules you wish to use
    // __real_send(fd, session_manager.getModulesAsBitArray(fd),
    //             session_manager.getBitArrayLen(), 0);

    if (session_manager.instantiateModules(fd, true)) {
        return -1;
    }    
    size_t total = 4096;
    ssize_t used = 0;
    char *buf = (char *)malloc(total);
    EncryptionModule *enc = (EncryptionModule *)session_manager.getFront(fd);
    while(!enc->handshake_done()) {
        enc->data_out(buf, (size_t*)&used, &total);
        if(__real_send(fd, (char *)&used, sizeof(size_t), 0) < 0) {return -1;}
        total = __real_send(fd, buf, used, 0);
        __real_fprintf(_log, "wanted to send %lu, only sent %lu\n", used, total);
        if(enc->handshake_done()) break;
        total = 4096;

        size_t incoming_size;
        while(__real_recv(fd, buf, sizeof(size_t), 0) < 0) {}
        //if(__real_recv(fd, buf, sizeof(size_t), 0) < 0) {return -1;}
        memcpy(&incoming_size, buf, sizeof(size_t));
        int accum = 0;
        while(accum < (int)incoming_size) {
            used = __real_recv(fd, buf+accum, incoming_size - accum, 0);
            if(used == -1) {
                __real_fprintf(_log,"errno: %s\n", strerror(errno));
            } else {
                accum += used;
            }
        }
        used = accum;
        __real_fprintf(_log, "recv %lu\n", used);
        enc->data_in(buf, (size_t*)&used, &total);
        total = 4096;
        used = 0;
    }
    free(buf);
    __real_fprintf(_log, "completely done with handshake. rc = %d\n", rc);

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
    //return new_fd;
    __real_fprintf(_log, "accept's fd: %d\n", new_fd);

    if (session_manager.instantiateModules(new_fd, false)) {
        return -1;
    }

    size_t total = 4096;
    ssize_t used = 0;
    char *buf = (char *)malloc(total);
    EncryptionModule *enc = (EncryptionModule *)session_manager.getFront(new_fd);
    __real_fprintf(_log, "starting handshake\n");
    while(!enc->handshake_done()) {
        total = 4096;
        size_t incoming_size;
        while(__real_recv(new_fd, buf, sizeof(size_t), 0) < 0) {}
        //if(__real_recv(new_fd, buf, sizeof(size_t), 0) < 0) {return -1;}
        memcpy(&incoming_size, buf, sizeof(size_t));
        int accum = 0;
        while(accum < (int)incoming_size) {
            used = __real_recv(new_fd, buf+accum, incoming_size - accum, 0);
            if(used == -1) {
                __real_fprintf(_log,"errno: %s\n", strerror(errno));
            } else {
                accum += used;
            }
        }
        used = accum;
        __real_fprintf(_log, "recv %lu bytes\n", used);
        enc->data_in(buf, (size_t*)&used, &total);
        if(enc->handshake_done()) break;
        total = 4096;
        used = 0;
        enc->data_out(buf, (size_t*)&used, &total);
        if(__real_send(new_fd, (char *)&used, sizeof(size_t), 0) < 0) {return -1;}
        total = __real_send(new_fd, buf, used, 0);
        __real_fprintf(_log, "wanted to send %lu, only sent %lu\n", used, total);
    }
    free(buf);
    __real_fprintf(_log, "completely done with handshake\n");

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

ssize_t write(int fd, const void *buf, size_t count)
{
    if(session_manager.known(fd)) {
        int rc;
        size_t original_size = count;
        TRACE();

        size_t buflen = 4096;
        char *new_buf = (char*)malloc(buflen);
        memcpy(new_buf, buf, count);
        if(session_manager.send(fd, new_buf, &count, &buflen) <= 0) {
            return -1;
        }
        if(__real_write(fd, (char *)&count, sizeof(size_t)) < 0) {return -1;}
        rc = __real_write(fd, new_buf, count);
        free(new_buf);
        __real_fprintf(_log, "want to write (unenc) %d, (enc) %d, sent %d\n", original_size, count, rc);

        if(rc <= 0) return rc; // HACK!!!
        return original_size;
    } else {
        return __real_write(fd, buf, count);
    }
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

ssize_t read(int fd, void *buf, size_t count) 
{
    if(session_manager.known(fd)) {
        ssize_t rc;
        TRACE();

        size_t buflen = 4096;
        char *new_buf = (char*)malloc(buflen);
        size_t incoming_size;
        if(__real_read(fd, new_buf, sizeof(size_t)) < 0) {return -1;}
        memcpy(&incoming_size, new_buf, sizeof(size_t));
        int accum = 0;
        while(accum < (int)incoming_size) {
            rc = __real_read(fd, new_buf+accum, incoming_size - accum);
            if(rc == -1) {
                __real_fprintf(_log,"errno: %s\n", strerror(errno));
            } else {
                accum += rc;
            }
        }
        __real_fprintf(_log, "read (enc) %d\n", rc);
        if (rc > 0) {
            // Need to be careful that we don't exceed buflen
            session_manager.recv(fd, new_buf, (size_t *)&rc, &buflen);
        }
        memcpy(buf, new_buf, count);
        free(new_buf);

        if(rc > 0 && (size_t)rc > count) return count; // HACK!!!
        __real_fprintf(_log, "read (unenc) %d\n", rc);
        return rc;
    } else {
        return __real_read(fd, buf, count);
    }
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
    if(session_manager.known(fd))
        TRACE();

	rc = __real_close(fd);
    session_manager.close(fd);

	return rc;
}
