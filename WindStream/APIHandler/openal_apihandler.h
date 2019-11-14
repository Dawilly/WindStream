#include <stdio.h>

#ifndef __OPENALHANDLER__INCLUDED__
#define __OPENALHANDLER__INCLUDED__

typedef struct oal_api OalApi;

OalApi* Oal_CreateInstance(int (*dylibopen)(char*, void*), int (*dylibsym)(void*, void**, char**, int));
  void  Oal_OpenDevice(OalApi*, char*);
  void  Oal_OpenDeviceWithSelection(OalApi*, int);
  void  Oal_CloseDevice(OalApi*);
   int  Oal_Get_Devices(OalApi*);
  void  Oal_PrintDevices(OalApi*);
  void  Oal_DestroyInstance(OalApi*);
   int  Oal_Get_DeviceCount(OalApi*);

#endif // !__OPENALHANDLER__INCLUDED__
