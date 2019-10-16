#include "apihandler.h"
#include "openal_apihandler.h"
#include "../WindStream.h"

typedef struct oalapi {
	void* device;
	void* ctx;

	void* (*OpenDevice)(char*);

	void* (*CreateContext)(void*, int*);
	char (*MakeContextCurrent)(void*);
	void* (*GetCurrentContext)();
	void* (*GetContextsDevice)(void*);
	void (*DestroyContext)(void*);

	char (*CloseDevice)(void*);
} OAL_API;

const int numberOfFuncs = 7;

void OpenDefaultDevice(OAL_API*);

OAL_API* CreateOalApi(int (*dylibopen)(char*, void*), int (*dylibsym)(void*, void**, char**, int)) {
	OAL_API* newAPI; 
	void* lib = NULL;
	void** funcList;
	char* names[7] = {
		"alcOpenDevice", "alcCreateContext", "alcMakeContextCurrent", 
		"alcGetCurrentContext", "alcGetContextsDevice", "alcDestroyContext", 
		"alcCloseDevice"
	};

	//To-do: Macro DLL/SO/DYLIB name, as it depends on OS.
	if (!dylibopen("OpenAL32.dll", &lib)) {
		fprintf(stderr, "Unable to find OpenAL32.dll.");
		return NULL;
	}

	funcList = malloc(sizeof(void*) * numberOfFuncs);
	if (!funcList) {
		fprintf(stderr, "Unable to allocate memory for void**");
		return;
	}
	
	//To-Do: Express what function was not found.
	if (!dylibsym(lib, funcList, names, numberOfFuncs)) {
		fprintf(stderr, "Unable to find functions.");
		return NULL;
	}

	newAPI = (OAL_API*)malloc(sizeof(OAL_API));
	if (!newAPI) {
		fprintf(stderr, "Unable to allocate memory for OAL_API*\n");
		return;
	}

	newAPI->OpenDevice = funcList[0];
	newAPI->CreateContext = funcList[1];
	newAPI->MakeContextCurrent = funcList[2];
	newAPI->GetCurrentContext = funcList[3];
	newAPI->GetContextsDevice = funcList[4];
	newAPI->DestroyContext = funcList[5];
	newAPI->CloseDevice = funcList[6];

	return newAPI;
}

void OpenDevice(OAL_API* ptr, char* deviceString) {
	if (ptr == NULL) return;
	
	ptr->device = ptr->OpenDevice(deviceString);

	if (!ptr->device) {
		fprintf(stderr, "Unable to open device: %s\n", deviceString);
		if (deviceString != NULL) OpenDefaultDevice(ptr);
	}

	ptr->ctx = ptr->CreateContext(ptr->device, NULL);

	if (ptr->ctx == NULL || ptr->MakeContextCurrent(ptr->ctx) == FALSE) {
		if (ptr->ctx != NULL) ptr->DestroyContext(ptr->ctx);
		ptr->CloseDevice(ptr->device);
		fprintf(stderr, "Unable to set context.\n");
		return;
	}

	// Extension goes here.
	return;
}

void CloseDevice(OAL_API* ptr) {
	void* currentCtx = ptr->GetCurrentContext();
	if (currentCtx == NULL) return;

	void* device = ptr->GetContextsDevice(currentCtx);

	ptr->MakeContextCurrent(NULL);
	ptr->DestroyContext(currentCtx);
	ptr->CloseDevice(device);
	return;
}

///////////////////////
// Private Functions //
///////////////////////

void OpenDefaultDevice(OAL_API* ptr) {
	ptr->device = ptr->OpenDevice(NULL);

	if (!ptr->device) {
		fprintf(stderr, "Unable to open default device!\n");
	}
	return;
}

void CleanUp(int count, ...) {
	va_list arg;
	va_start(arg, count);
	for (int i = 0; i < count; i++) {
		free(va_arg(arg, void*));
	}
	va_end(arg);
}