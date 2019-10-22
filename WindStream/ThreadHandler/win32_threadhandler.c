#include "win32_threadhandler.h"
#include "../WindStream.h"

typedef struct win32_thread {
	unsigned int hThread;
	void* mutex;
	int threadState;
	void (*functionBootstrap)(void*);

	void* security;
	unsigned int stackSize;
	void* argList;
	unsigned int initFlag;
	unsigned int threadId;
} WIN32THREAD;

WIN32THREAD* CreateWin32Thread() {
	WIN32THREAD* newThread = malloc(sizeof(WIN32THREAD));
	if (newThread == NULL) return NULL;

	newThread->threadState = UNSET;
	newThread->mutex = NULL;
	newThread->functionBootstrap = NULL;
	newThread->security = NULL;
	newThread->stackSize = 0;
	newThread->argList = NULL;
	newThread->initFlag = 0;
	newThread->threadId = 0;

	return newThread;
}

void SetFunction(WIN32THREAD* ptr, void (*function)(void*)) {
	if (function == NULL) return;

	ptr->functionBootstrap = function;
	ptr->threadState = STOPPED;
	return;
}

BOOL StartThread(WIN32THREAD* ptr) {
	if (ptr->threadState == UNSET) return FALSE;

	ptr->hThread = _beginthreadex(
		ptr->security, 
		ptr->stackSize,
		ptr->functionBootstrap, 
		ptr->argList,
		ptr->initFlag, 
		&(ptr->threadId)
	);

	if (ptr->hThread == 0) return FALSE;

	ptr->threadState = RUNNING;
	return TRUE;
}