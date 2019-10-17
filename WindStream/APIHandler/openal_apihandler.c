#include "apihandler.h"
#include "openal_apihandler.h"
#include "../WindStream.h"

typedef struct oalapi {
	char** deviceList;
	void* device;
	void* ctx;

	void* (*OpenDevice)(char*);

	void* (*CreateContext)(void*, int*);
	char (*MakeContextCurrent)(void*);
	void* (*GetCurrentContext)();
	void* (*GetContextsDevice)(void*);
	void (*DestroyContext)(void*);

	char (*IsExtensionPresent)(void*, char*);

	char* (*GetString)(void*, int);

	char (*CloseDevice)(void*);
} OAL_API;

const int numberOfFuncs = 9;

void openDefaultDevice(OAL_API*);
void iterateDeviceList(OAL_API*, int);
int countDeviceList(const char*);
void cleanUp(int, ...);

OAL_API* CreateOalApi(int (*dylibopen)(char*, void*), int (*dylibsym)(void*, void**, char**, int)) {
	OAL_API* newAPI; 
	void* lib = NULL;
	void** funcList;
	char* names[] = {
		"alcOpenDevice", "alcCreateContext", "alcMakeContextCurrent", 
		"alcGetCurrentContext", "alcGetContextsDevice", "alcDestroyContext", 
		"alcCloseDevice", "alcIsExtensionPresent", "alcGetString"
	};

	//To-do: Macro DLL/SO/DYLIB name, as it depends on OS.
	if (!dylibopen(LIBRARYNAME, &lib)) {
		fprintf(stderr, "Unable to find OpenAL32.dll.");
		return NULL;
	}

	funcList = malloc(sizeof(void*) * numberOfFuncs);
	if (!funcList) {
		fprintf(stderr, "Unable to allocate memory for void**");
		return NULL;
	}
	
	//To-Do: Express what function was not found.
	if (!dylibsym(lib, funcList, names, numberOfFuncs)) {
		fprintf(stderr, "Unable to find functions.");
		return NULL;
	}

	newAPI = (OAL_API*)malloc(sizeof(OAL_API));
	if (!newAPI) {
		fprintf(stderr, "Unable to allocate memory for OAL_API*\n");
		return NULL;
	}

	newAPI->OpenDevice = funcList[0];
	newAPI->CreateContext = funcList[1];
	newAPI->MakeContextCurrent = funcList[2];
	newAPI->GetCurrentContext = funcList[3];
	newAPI->GetContextsDevice = funcList[4];
	newAPI->DestroyContext = funcList[5];
	newAPI->CloseDevice = funcList[6];
	newAPI->IsExtensionPresent = funcList[7];
	newAPI->GetString = funcList[8];

	free(funcList);

	return newAPI;
}

void GetAllDevices(OAL_API* ptr) {
	if (ptr->IsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT") != FALSE) {
		int count = countDeviceList(ptr->GetString(NULL, 0x1013));
		iterateDeviceList(ptr, 0x1013, count);
	}
}

void PrintAllDevices(OAL_API* ptr) {
	for (int i = 0; i < 4; i++) {
		printf("\t%s\n", ptr->deviceList[i]);
	}
}

void OpenDevice(OAL_API* ptr, char* deviceString) {
	if (ptr == NULL) return;
	
	ptr->device = ptr->OpenDevice(deviceString);

	if (!ptr->device) {
		fprintf(stderr, "Unable to open device: %s\n", deviceString);
		if (deviceString != NULL) openDefaultDevice(ptr);
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

void DestroyOalApi(OAL_API* ptr) {

}

///////////////////////
// Private Functions //
///////////////////////

int countDeviceList(const char* list) {
	int count = 0;
	if (!list || *list == '\0') {
		printf("DeviceList: No Audio Device(s) found...\n");
		return 0;
	}
	else {
		do {
			list += strlen(list) + 1;
			count++;
		} while (*list != '\0');
	}
	return count;
}

void iterateDeviceList(OAL_API* ptr, int enumValue, int count) {
	char* list = ptr->GetString(NULL, enumValue);
	
	ptr->deviceList = (char**)malloc(sizeof(char*) * count);
	for (int i = 0; i < count; i++) {
		ptr->deviceList[i] = (char*)malloc(sizeof(char) * strlen(list) + 1);
		strcpy(ptr->deviceList[i], list);
		list += strlen(list) + 1;
	}

	return;
}

void openDefaultDevice(OAL_API* ptr) {
	ptr->device = ptr->OpenDevice(NULL);

	if (!ptr->device) {
		fprintf(stderr, "Unable to open default device!\n");
	}
	return;
}

void cleanUp(int count, ...) {
	va_list arg;
	va_start(arg, count);
	for (int i = 0; i < count; i++) {
		free(va_arg(arg, void*));
	}
	va_end(arg);
}