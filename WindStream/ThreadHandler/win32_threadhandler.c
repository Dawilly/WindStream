#include "win32_threadhandler.h"

typedef struct win32_thread {
	unsigned int threadPtr;
} WIN32THREAD;

WIN32THREAD* CreateWin32Thread() {
	WIN32THREAD* newThread = malloc(sizeof(WIN32THREAD));
	if (newThread == NULL) return NULL;

	newThread->threadPtr = _beginthread(NULL, 0, 0);

	return newThread;
}