#include "apihandler.h"
#include "openal_apihandler.h"
#include "../WindStream.h"

typedef struct oal_api {
	char** deviceList;
	int deviceCount;

	void* device;
	void* ctx;
	int deviceState;

	void* (*OpenDevice)(char*);

	void* (*CreateContext)(void*, int*);
	char  (*MakeContextCurrent)(void*);
	void* (*GetCurrentContext)();
	void* (*GetContextsDevice)(void*);
	void  (*DestroyContext)(void*);

	void (*GenerateBuffers)(int, unsigned int*);
	void (*BufferData)(unsigned int, int, void*, int, int);
	void (*BufferInt)(unsigned int, int, int);
	void (*BufferFloat)(unsigned int, int, float);
	void (*DeleteBuffers)(int, unsigned int*);

	void (*GenerateSources)(int, unsigned int*);
	void (*GetSourceInt)(unsigned int, int, int*);
	void (*SourceInt)(unsigned int, int, int);
	void (*SourceFloat)(unsigned int, int, float);
	void (*SourceThreeFloats)(unsigned int, int, float, float, float);
	void (*SourcePlay)(unsigned int);
	void (*DeleteSources)(int, unsigned int*);

	void (*ListenerThreeFloats)(int, float, float, float);
	void (*ListenerFloatValue)(int, float*);

	char (*IsExtensionPresent)(void*, char*);

	char* (*GetString)(void*, int);
	int   (*GetError)();

	char (*CloseDevice)(void*);
} OALAPI;

const int numberOfFuncs = 24;

void openDefaultDevice(OALAPI*);
void iterateDeviceList(OALAPI*, int);
int countDeviceList(const char*);
void cleanUp(int, ...);

OALAPI* CreateOalApi(int (*dylibopen)(char*, void*), int (*dylibsym)(void*, void**, char**, int)) {
	OALAPI* newAPI;
	void* lib = NULL;
	void** funcList;
	char* names[] = {
		"alcOpenDevice",			// 1 
		"alcCreateContext",			// 2
		"alcMakeContextCurrent",	// 3
		"alcGetCurrentContext",		// 4
		"alcGetContextsDevice",		// 5
		"alcDestroyContext",		// 6
		"alcCloseDevice",			// 7
		"alcIsExtensionPresent",	// 8
		"alcGetString",				// 9
		"alGetError",				// 10
		"alGenBuffers",				// 11
		"alDeleteBuffers",			// 12
		"alBufferData",				// 13
		"alGenSources",				// 14
		"alSourcef",				// 15
		"alSourcei",				// 16
		"alBufferi",				// 17
		"alBufferf",				// 18
		"alSource3f",				// 19
		"alSourcePlay",				// 20
		"alDeleteSources",			// 21
		"alGetSourcei",				// 22
		"alListener3f",				// 23
		"alListenerfv"				// 24
	};

	if (!dylibopen(LIBRARYNAME, &lib)) {
		fprintf(stderr, "Unable to find %s\n.", LIBRARYNAME);
		return NULL;
	}

	funcList = malloc(sizeof(void*) * numberOfFuncs);
	if (!funcList) {
		fprintf(stderr, "Unable to allocate memory for void**\n");
		return NULL;
	}
	
	//To-Do: Express what function was not found.
	if (!dylibsym(lib, funcList, names, numberOfFuncs)) {
		fprintf(stderr, "Unable to find functions.\n");
		return NULL;
	}

	newAPI = (OALAPI*)malloc(sizeof(OALAPI));
	if (!newAPI) {
		fprintf(stderr, "Unable to allocate memory for OAL_API*\n");
		return NULL;
	}

	int i = 0;

	newAPI->OpenDevice = funcList[i++];
	newAPI->CreateContext = funcList[i++];
	newAPI->MakeContextCurrent = funcList[i++];
	newAPI->GetCurrentContext = funcList[i++];
	newAPI->GetContextsDevice = funcList[i++];
	newAPI->DestroyContext = funcList[i++];
	newAPI->CloseDevice = funcList[i++];
	newAPI->IsExtensionPresent = funcList[i++];
	newAPI->GetString = funcList[i++];
	newAPI->GetError = funcList[i++];
	newAPI->GenerateBuffers = funcList[i++];
	newAPI->DeleteBuffers = funcList[i++];
	newAPI->BufferData = funcList[i++];
	newAPI->GenerateSources = funcList[i++];
	newAPI->SourceFloat = funcList[i++];
	newAPI->SourceInt = funcList[i++];
	newAPI->BufferInt = funcList[i++];
	newAPI->BufferFloat = funcList[i++];
	newAPI->SourceThreeFloats = funcList[i++];
	newAPI->SourcePlay = funcList[i++];
	newAPI->DeleteSources = funcList[i++];
	newAPI->GetSourceInt = funcList[i++];
	newAPI->ListenerThreeFloats = funcList[i++];
	newAPI->ListenerFloatValue = funcList[i++];

	newAPI->deviceState = 0;

	free(funcList);

	return newAPI;
}

void GetAllDevices(OALAPI* ptr) {
	if (ptr->IsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT") != FALSE) {
		ptr->deviceCount = countDeviceList(ptr->GetString(NULL, 0x1013));
		iterateDeviceList(ptr, 0x1013);
	}
	return;
}

int GetDeviceCount(OALAPI* ptr) {
	return ptr->deviceCount;
}

void PrintAllDevices(OALAPI* ptr) {
	int count = ptr->deviceCount;
	for (int i = 0; i < count; i++) {
		printf("\t%d) %s\n", i, ptr->deviceList[i]);
	}
}

void OpenDeviceWithSelection(OALAPI* ptr, int indexSelection) {
	char* value = (indexSelection < 0 || indexSelection >= ptr->deviceCount) 
		? NULL : ptr->deviceList[indexSelection];
	OpenDevice(ptr, value);
	return;
}

void OpenDevice(OALAPI* ptr, char* deviceString) {
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

	ptr->deviceState = 1;
	// Extension goes here.
	return;
}

void CloseDevice(OALAPI* ptr) {
	void* currentCtx = ptr->GetCurrentContext();
	if (currentCtx == NULL) return;

	void* device = ptr->GetContextsDevice(currentCtx);

	ptr->MakeContextCurrent(NULL);
	ptr->DestroyContext(currentCtx);
	ptr->CloseDevice(device);
	ptr->deviceState = 0;
	return;
}

void DestroyOalApi(OALAPI* ptr) {
	if (ptr->deviceState != 0) {
		fprintf(stderr, "Device must be closed prior to object destruction.\n");
		return;
	}

	int count = ptr->deviceCount;
	for (int i = 0; i < count; i++) free(ptr->deviceList[i]);
	free(ptr->deviceList);
	// It seems we must let OpenAL take care of itself?
	//free(ptr->ctx);
	//free(ptr->device);

	free(ptr);
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

void iterateDeviceList(OALAPI* ptr, int enumValue) {
	char* list = ptr->GetString(NULL, enumValue);
	int count = ptr->deviceCount;

	ptr->deviceList = (char**)malloc(sizeof(char*) * count);

	if (!ptr->deviceList) {
		fprintf(stderr, "Unable to allocate memory: DeviceList\n");
		return;
	}

	for (int i = 0; i < count; i++) {
		ptr->deviceList[i] = (char*)malloc(sizeof(char) * strlen(list) + 1);
		strcpy(ptr->deviceList[i], list);
		list += strlen(list) + 1;
	}

	return;
}

void openDefaultDevice(OALAPI* ptr) {
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
