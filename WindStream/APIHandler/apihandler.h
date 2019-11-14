#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef __APIHANDLER__INCLUDED__
#define __APIHANDLER__INCLUDED__

typedef struct api_handler {
	void* instance;
	void* (*Create)(int (*dylibopen)(char*, void*), int (*dylibsym)(void*, void**, char**, int));
	void  (*Open)(void*, void*);
	void  (*Close)(void*);
	void  (*Destroy)(void*);

	int   (*Get_DeviceCount)(void*);
	int   (*Get_Devices)(void*);
	void  (*PrintDevices)(void*);
} ApiHandler;

ApiHandler* CreateInstance(ApiType);

#endif // !__APIHANDLER__INCLUDED__
