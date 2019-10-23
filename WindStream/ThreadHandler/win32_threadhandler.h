#include <stdio.h>
#include <stdlib.h>
#include "threadhandler.h"

#ifndef __WIN32THREADHANDLER__INCLUDED__
#define __WIN32THREADHANDLER__INCLUDED__

#if defined(_WIN32)
#include <windows.h>
#include <process.h>
#endif

typedef struct win32_thread WIN32THREAD;

#endif //!__WIN32THREADHANDLER__INCLUDED__