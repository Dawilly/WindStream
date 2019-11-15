#include "WindStream.h"
#include "APIHandler/apihandler.h"
#include "FileHandler/wave_filehandler.h"
#include "ThreadHandler/win32_threadhandler.h"

void testFunction(void* test) {
	while (1) {
		_sleep(50);
	}
}

int main() {
	WaveFile* file = CreateWavFile();
	ApiHandler* apiPtr = CreateInstance(OpenAL);
	int count = apiPtr->Get_Devices(apiPtr->instance);
	int input;

	printf("Devices for SMAPI to use for Audio...\n");
	apiPtr->PrintDevices(apiPtr->instance);

	printf("\n\nSelect a device: ");
	ScanForIntInRange(stdin, &input, 0, count);

	printf("Opening Device...\n");
	apiPtr->Open(apiPtr->instance, input);

	printf("Opening whistle.wav...\n");
	OpenWavFile(file, "whistle.wav");

	printf("Reading wav header...\n\n");
	ReadWavHeader(file);

	printf("Closing...\n");
	apiPtr->Close(apiPtr->instance);

	printf("Destroying...\n");
	apiPtr->Destroy(apiPtr->instance);
	free(apiPtr);

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