#include <stdio.h>
#include "apihandler.h"

#ifndef __OPENALHANDLER__INCLUDED__
#define __OPENALHANDLER__INCLUDED__

typedef struct oal_api OALAPI;

OALAPI* CreateOalApi(int (*dylibopen)(char*, void*), int (*dylibsym)(void*, void**, char**, int));
void OpenDevice(OALAPI*, char*);
void OpenDeviceWithSelection(OALAPI*, int);
void CloseDevice(OALAPI*);
void GetAllDevices(OALAPI*);
void PrintAllDevices(OALAPI*);
void DestroyOalApi(OALAPI*);

#endif // !__OPENALHANDLER__INCLUDED__
