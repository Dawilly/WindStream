#include "apihandler.h"
#include "openal_apihandler.h"
#include "../WindStream.h"
#include "../helpers.h"

ApiHandler* CreateOpenALInstance(ApiHandler*);

ApiHandler* CreateInstance(ApiType apiType) {
	ApiHandler* newHandler = Cmalloc(sizeof(ApiHandler), "ApiHandler.CreateInstance", TRUE);

	switch (apiType) {
		case Auto:
			return NULL;
		case OpenAL:
			return CreateOpenALInstance(newHandler);
		case PortAudio:
			return NULL;
		case OSX_CoreAudio:
			return NULL;
		case WIN_XAudio2:
			return NULL;
		case NIX_Alsa:
			return NULL;
		default:
			return NULL;
	}
}

/////////////////////////
/// Private Functions ///
/////////////////////////

ApiHandler* CreateOpenALInstance(ApiHandler* ptr) {
	ptr->instance = Oal_CreateInstance(GetLibrary, GetFunctionAddresses);
	ptr->Create = Oal_CreateInstance;
	ptr->Open = Oal_OpenDeviceWithSelection;
	ptr->Close = Oal_CloseDevice;
	ptr->Destroy = Oal_DestroyInstance;
	ptr->Get_DeviceCount = Oal_Get_DeviceCount;
	ptr->Get_Devices = Oal_Get_Devices;
	ptr->PrintDevices = Oal_PrintDevices;
	return ptr;
}
