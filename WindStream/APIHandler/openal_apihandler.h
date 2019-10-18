#include <stdio.h>
#include "apihandler.h"

#ifndef __OPENALHANDLER__INCLUDED__
#define __OPENALHANDLER__INCLUDED__

typedef struct oalapi OAL_API;

OAL_API* CreateOalApi(int (*dylibopen)(char*, void*), int (*dylibsym)(void*, void**, char**, int));
void OpenDevice(OAL_API*, char*);
void OpenDeviceWithSelection(OAL_API*, int);
void CloseDevice(OAL_API*);
void GetAllDevices(OAL_API*);
void PrintAllDevices(OAL_API*);
void DestroyOalApi(OAL_API*);

#endif // !__OPENALHANDLER__INCLUDED__
