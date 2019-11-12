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
#define LIBRARYNAME "libopenal.so"
#endif

typedef enum apitype {
	Auto,
	OpenAL,
	PortAudio,
	OSX_CoreAudio,
	WIN_XAudio2,
	NIX_Alsa
} ApiType;

typedef enum filetype {
	Wave,
	Flac,
	Ogg
} FileType;

typedef enum os {
	Windows,
	OSX,
	Linux,
	Unknown
} OS;

bool GetFunctionAddresses(void*, void**, char**, int);
bool GetLibrary(char*, void**);

#endif
