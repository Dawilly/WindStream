// WindStream.h : Include file for standard system include files,
// or project specific include files.

#ifndef __WINDSTREAM__INCLUDED__
#define __WINDSTREAM__INCLUDED__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) 
#include <windows.h>
#define GETLIBFUNCTION GetProcAddress
#define LIBRARYNAME "OpenAL32.dll"
#else
#include <dlfcn.h>
#define GETLIBFUNCTION dlsym
#endif

#define TRUE 1
#define FALSE 0
#define BOOL int
#define NOAUDIODEVICE NULL

BOOL GetFunctionAddresses(void*, void**, char**, int);
BOOL GetLibrary(char*, void**);

#endif
