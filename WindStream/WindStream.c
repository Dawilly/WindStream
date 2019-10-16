// WindStream.c : Defines the entry point for the application.
//

#include "WindStream.h"

BOOL GetFunctionAddresses(void* lib, void** ptrArray, char** funcNames, int count) {
	int i;
	
	if (lib != NULL) {
		for (i = 0; i < count; i++) {
			ptrArray[i] = GETLIBFUNCTION(lib, funcNames[i]);
			if (ptrArray[i] == NULL) return FALSE;
		}
	}

	return TRUE;
}

#if defined(_WIN32)
BOOL GetLibrary(char* filePath, void** out) {
	(*out) = LoadLibrary(filePath);
	return ((*out) != NULL);
}
#else
BOOL GetLibrary(char* filePath, void** out) {
	(*out) = dlopen(filePath, RTLD_LAZY);
	return ((*out) != NULL);
}
#endif