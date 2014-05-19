#ifndef LOGGING_H
#define LOGGING_H

#define V_ERROR 0
#define V_WARNING 1
#define V_INFO 2
#define V_DEBUG 3

#ifdef DEBUG
#define VERBOSITY V_DEBUG
#else
#define VERBOSITY V_DEBUG
#endif

#define LOG(levelstr, color, s) fprintf(stderr, "\033[0;3%dm[ %s ]\033[0m\t[%s:%d (thread %p)]\t%s\n", color, levelstr, __FILE__, __LINE__, (void*)pthread_self(), s)
#define LOGF(levelstr, color, fmt, ...) fprintf(stderr, "\033[0;3%dm[ %s ]\033[0m\t[%s:%d (thread %p)]\t" fmt"\n", color, levelstr, __FILE__, __LINE__, (void*)pthread_self(), __VA_ARGS__) 

#if VERBOSITY >= V_INFO
#define INFO(s) LOG("INFO", 2, s)
#define INFOF(fmt, ...) LOGF("INFO", 2, fmt, __VA_ARGS__)
#else
#define INFO(s)
#define INFOF(fmt, ...)
#endif

#if VERBOSITY >= V_DEBUG
#define DBG(s) LOG("DEBUG", 4, s)
#define DBGF(fmt, ...) LOGF("DEBUG", 4, fmt, __VA_ARGS__)
#else
#define DBG(s)
#define DBGF(fmt, ...)
#endif

#if VERBOSITY >= V_ERROR
#define ERROR(s) LOG("ERROR", 1, s)
#define ERRORF(fmt, ...) LOGF("ERROR", 1, fmt, __VA_ARGS__)
#else
#define ERROR(s)
#define ERRORF(fmt, ...)
#endif

#if VERBOSITY >= V_WARNING
#define WARN(s) LOG("WARNING", 3, s)
#define WARNF(fmt, ...) LOGF("WARNING", 3, fmt, __VA_ARGS__)
#else
#define WARN(s)
#define WARNF(fmt, ...)
#endif

#endif /* LOGGING_H */
