#ifndef XWRAP_H
#define XWRAP_H


//#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "Manager.h"

#define XIA_PRELOAD

#define TRACE()		{if (_log_trace)   __real_fprintf(_log, "xwrap: %s\r\n", __FUNCTION__);}
#define STOCK()		{if (_log_trace)   __real_fprintf(_log, "xwrap: %s informational tracing only\r\n", __FUNCTION__);}

#define MSG(...)	{if (_log_info)    __real_fprintf(_log, "xwrap: %s ", __FUNCTION__); __real_fprintf(_log, __VA_ARGS__);}

#define WARNING(...)	{if (_log_warning) __real_fprintf(_log, "xwrap: %s ", __FUNCTION__); __real_fprintf(_log, __VA_ARGS__);}

/*
** If XIA_PRELOAD is defined, the code below will be compiled as a library that
** can be loaded at runtime into an application giving it XIA support without
** requiring a recompile. Seed above for functions that require code changes.
**
# see the xia-core/bin/xwrap script for usage
*/
#ifdef XIA_PRELOAD

/*
** Declare a typedef and a function pointer definition that uses it.
** The typedef is created so we can use it to cast the void * result from
** dlsym to the appropriate function pointer type in the GET_FCN macro.
**
** example:
**   DECLARE(int, socket, int domain, int type, int protocol); ==>
**
**   typedef int (*fcn_socket)(int domain, int type, int protocol);
**   fcn_socket __real_socket;
*/
#define DECLARE(r, f, ...)	\
	typedef r (*fcn_##f)(__VA_ARGS__);	\
	fcn_##f __real_##f

/*
** assign value of real function to a pointer so we can call it from
** the wrapper functions.
**
** example:
**   GET_FCN(socket) ==>
**
**   __real_socket = (fcn_socket)dlsym(RTLD_NEXT, "socket");
*/
#define GET_FCN(f)	__real_##f = (fcn_##f)dlsym(RTLD_NEXT, #f)

/*
**  using LD_PRELOAD method, variables for each overridden function
*/

// basic socket calls
DECLARE(int, socket, int domain, int type, int protocol);
DECLARE(int, bind, int fd, const struct sockaddr *addr, socklen_t len);
DECLARE(int, connect, int fd, const struct sockaddr *addr, socklen_t len);
DECLARE(ssize_t, send, int fd, const void *buf, size_t n, int flags);
DECLARE(ssize_t, recv, int fd, void *buf, size_t n, int flags);
DECLARE(ssize_t, write, int fd, const void *buf, size_t count);
DECLARE(ssize_t, read, int fd, void *buf, size_t count);
DECLARE(ssize_t, sendto, int fd, const void *buf, size_t n, int flags, const struct sockaddr *addr, socklen_t addr_len);
DECLARE(ssize_t, recvfrom, int fd, void *buf, size_t n, int flags, struct sockaddr *addr, socklen_t *addr_len);
DECLARE(int, listen, int fd, int n);
DECLARE(int, accept, int fd, struct sockaddr *addr, socklen_t *addr_len);
DECLARE(int, close, int fd);

DECLARE(int, fprintf, FILE *stream, const char *format, ...);

//****************************************************************************
// set up logging parameters
//
int _log_trace = 0;
int _log_warning = 0;
int _log_info = 0;
FILE *_log = NULL;

void __xwrap_setup()
{
	if (getenv("XWRAP_TRACE") != NULL)
		_log_trace = 1;
	if (getenv("XWRAP_VERBOSE") != NULL)
		_log_trace = _log_info = _log_warning = 1;
	if (getenv("XWRAP_INFO") != NULL)
		_log_info = 1;
	if (getenv("XWRAP_WARNING") != NULL)
		_log_warning = 1;

	const char *lf = getenv("XWRAP_LOGFILE");
	if (lf)
		_log = fopen(lf, "w");
	if (!_log)
		_log = stderr;
}


/******************************************************************************
**
** Called at library load time to initialize the function pointers
**
******************************************************************************/
void __attribute__ ((constructor)) xwrap_init(void)
{
	__xwrap_setup();

	if (_log_info || _log_warning || _log_trace)
		fprintf(_log, "loading wrappers (created: %s)\n", __DATE__);

	GET_FCN(socket);
	GET_FCN(bind);
	GET_FCN(connect);
	GET_FCN(send);
	GET_FCN(recv);
    GET_FCN(write);
    GET_FCN(read);
	GET_FCN(sendto);
	GET_FCN(recvfrom);
	GET_FCN(listen);
	GET_FCN(accept);
	GET_FCN(close);

	GET_FCN(fprintf);
}


//char **session_buffers;

#endif // XIA_PRELOAD



#endif /* XWRAP_H */
