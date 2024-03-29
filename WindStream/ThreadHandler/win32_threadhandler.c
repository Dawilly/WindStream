#include "win32_threadhandler.h"
#include "../WindStream.h"

typedef struct win32_thread {
	void* hThread;
	void* lock;
	int threadState;
	void (*functionCall)(void*);

	void* security;
	unsigned int stackSize;
	void* argList;
	unsigned int initFlag;
	unsigned int threadId;

	int signal;
} WIN32THREAD;

void Bootstrapper(void*);

WIN32THREAD* CreateWin32Thread() {
	WIN32THREAD* newThread = malloc(sizeof(WIN32THREAD));
	if (newThread == NULL) return NULL;

	newThread->threadState = UNSET;
	newThread->lock = NULL;
	newThread->functionCall = NULL;
	newThread->security = NULL;
	newThread->stackSize = 0;
	newThread->argList = NULL;
	newThread->initFlag = 0;
	newThread->threadId = 0;
	newThread->signal = 0;

	return newThread;
}

void SetFunction(WIN32THREAD* ptr, void (*function)(void*)) {
	if (function == NULL) return;

	ptr->functionCall = function;
	ptr->threadState = STOPPED;
	return;
}

bool StartThread(WIN32THREAD* ptr) {
	if (ptr->threadState == UNSET) return FALSE;

	ptr->hThread = CreateThread(
		ptr->security, 
		ptr->stackSize,
		(void(*)(void*))Bootstrapper,
		(void*)ptr,
		ptr->initFlag, 
		&(ptr->threadId)
	);

	if (ptr->hThread == 0) return FALSE;

	ptr->signal = 1;
	ptr->threadState = RUNNING;

	return TRUE;
}

void Bootstrapper(void* ptr) {
	WIN32THREAD* winPtr = (WIN32THREAD*)ptr;
	while (winPtr->signal) {
		winPtr->functionCall(winPtr);
	}
}

void StopThread(WIN32THREAD* ptr) {
	ptr->signal = 0;
}

void SetResource() {

}

void CreateLock(WIN32THREAD* ptr) {
	ptr->lock = CreateMutexW(NULL, FALSE, NULL);
}