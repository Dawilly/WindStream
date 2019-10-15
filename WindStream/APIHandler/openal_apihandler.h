#include <stdio.h>
#include "apihandler.h"

#ifndef __OPENALHANDLER__INCLUDED__
#define __OPENALHANDLER__INCLUDED__

typedef struct oalapi OAL_API;

OAL_API* CreateOalApi(int (*dylibopen)(char*, void*), int (*dylibsym)(void*, void**, char**, int));

#endif // !__OPENALHANDLER__INCLUDED__
