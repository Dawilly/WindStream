#include "apihandler.h"
#include "openal_apihandler.h"

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
	if (!dylibopen("OpenAL32.dll", lib)) {
		fprintf(stderr, "Unable to find OpenAL32.dll.");
		return NULL;
	}

	funcList = malloc(sizeof(void*) * numberOfFuncs);
	
	//To-Do: Express what function was not found.
	if (!dylibsym(lib, funcList, names, numberOfFuncs)) {
		fprintf(stderr, "Unable to find functions.");
		return NULL;
	}

	newAPI = (OAL_API*)malloc(sizeof(OAL_API));

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


}

void CloseDevice(OAL_API* ptr) {
	
}

///////////////////////
// Private Functions //
///////////////////////



void CleanUp(int count, ...) {
	va_list arg;
	va_start(arg, count);
	for (int i = 0; i < count; i++) {
		free(va_arg(arg, void*));
	}
	va_end(arg);
}