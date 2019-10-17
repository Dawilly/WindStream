#include "WindStream.h"
#include "openal_apihandler.h"

int main() {
	printf("Devices for SMAPI to use for Audio...\n");

	OAL_API* test = CreateOalApi(GetLibrary, GetFunctionAddresses);
	GetAllDevices(test);
	PrintAllDevices(test);

	return 0;
}