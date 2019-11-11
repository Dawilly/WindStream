#include "../WindStream/WindStream.h"
#include "../WindStream/APIHandler/openal_apihandler.h"
#include "../WindStream/FileHandler/wav_filehandler.h"
#include "../WindStream/ThreadHandler/win32_threadhandler.h"

void testFunction(void* test) {
	while (1) {
		_sleep(50);
	}
}

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

	printf("Testing threads...");
	WIN32THREAD* thread = CreateWin32Thread();
	SetFunction(thread, testFunction);

	if (StartThread(thread)) {
		while (1) {
			_sleep(50);
		}
	} else {
		printf("No thread lol.\n");
	}

	return 0;
}