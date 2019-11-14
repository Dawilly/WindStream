// WindStream.c : Defines the entry point for the application.

#include "WindStream.h"
#include "APIHandler/apihandler.h"

typedef struct windstreamSettings {
	void* ApiCalls;
	void* ThreadCalls;
} WSettings;

void WindStreamImpl();
void* SetupApiCalls(ApiType);

void WindStream(ApiType apiType) {
	WSettings* settings = Cmalloc(sizeof(WSettings), "WindStream.settings", TRUE);
	settings->ApiCalls = SetupApiCalls(apiType);
	
}

void GetInfo() {

}

///////////////////////
// Private Functions //
///////////////////////

void WindStreamImpl() {
	bool Running = TRUE;

	while (Running) {

	}
}

void* SetupApiCalls(ApiType apiType) {

}

bool GetFunctionAddresses(void* lib, void** ptrArray, char** funcNames, int count) {
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
bool GetLibrary(char* filePath, void** out) {
	(*out) = LoadLibrary(filePath);
	return ((*out) != NULL);
}
#else
bool GetLibrary(char* filePath, void** out) {
	(*out) = dlopen(filePath, RTLD_LAZY);
	return ((*out) != NULL);
}
#endif
