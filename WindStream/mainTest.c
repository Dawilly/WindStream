#include "WindStream.h"
#include "openal_apihandler.h"

int main() {
	int input;
	printf("Devices for SMAPI to use for Audio...\n");

	OAL_API* test = CreateOalApi(GetLibrary, GetFunctionAddresses);
	GetAllDevices(test);
	PrintAllDevices(test);

	printf("\n\nSelect a device: ");
	scanf("%d", &input);

	printf("Opening...\n");
	OpenDeviceWithSelection(test, input);
	printf("Closing...\n");
	CloseDevice(test);
	printf("Destroying...\n");
	DestroyOalApi(test);

	return 0;
}