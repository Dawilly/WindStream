#include "WindStream.h"
#include "openal_apihandler.h"
#include "wav_filehandler.h"

int main() {
	int input;
	int count;
	OALAPI* test = CreateOalApi(GetLibrary, GetFunctionAddresses);
	WAVFILE* file = CreateWavFile();

	printf("Devices for SMAPI to use for Audio...\n");
	GetAllDevices(test);
	PrintAllDevices(test);
	count = GetDeviceCount(test);

	printf("\n\nSelect a device: ");
	do {
		scanf("%d", &input);
		if (input < 0 || input > count) {
			printf("Invalid input. Try again.\n");
		}
	} while (input < 0 || input > count);

	printf("Opening Device...\n");
	OpenDeviceWithSelection(test, input);
	printf("Opening whistle.wav...\n");
	OpenWavFile(file, "whistle.wav");
	printf("Reading wav header...\n\n");
	ReadWavHeader(file);
	printf("\nSampling not yet implemented!\n");
	printf("Closing...\n");
	CloseDevice(test);
	printf("Destroying...\n");
	DestroyOalApi(test);

	return 0;
}