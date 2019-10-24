// WindStream.h : Include file for standard system include files,
// or project specific include files.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definitions.h"

#ifndef __WINDSTREAM__INCLUDED__
#define __WINDSTREAM__INCLUDED__


#if defined(_WIN32) 
#include <windows.h>
#define GETLIBFUNCTION GetProcAddress
#define LIBRARYNAME "OpenAL32.dll"
#else
#include <dlfcn.h>
#define GETLIBFUNCTION dlsym
#endif

BOOL GetFunctionAddresses(void*, void**, char**, int);
BOOL GetLibrary(char*, void**);

#endif
