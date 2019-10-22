#include <stdio.h>
#include <stdlib.h>

#ifndef __WIN32THREADHANDLER__INCLUDED__
#define __WIN32THREADHANDLER__INCLUDED__

#if defined(_WIN32)
#include <windows.h>
#include <process.h>
#endif

typedef struct win32_thread WIN32THREAD;

#define UNSET -1
#define STOPPED 0
#define RUNNING 1

#endif //!__WIN32THREADHANDLER__INCLUDED__