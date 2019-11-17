#include "WindStream.h"
#include "APIHandler/apihandler.h"
#include "FileHandler/filehandler.h"
#include "ThreadHandler/win32_threadhandler.h"

void testFunction(void* test) {
	while (1) {
		_sleep(50);
	}
}

int main() {

	FileHandler* file = File_CreateInstance(Wave);
	ApiHandler* apiPtr = Api_CreateInstance(OpenAL);
	int count = apiPtr->Get_Devices(apiPtr->instance);
	int input;

	printf("Devices for SMAPI to use for Audio...\n");
	apiPtr->PrintDevices(apiPtr->instance);

	printf("\n\nSelect a device: ");
	ScanForIntInRange(stdin, &input, 0, count);

	printf("Opening Device...\n");
	apiPtr->Open(apiPtr->instance, input);

	printf("Opening whistle.wav...\n");
	file->Open(file->instance, "whistle.wav");

	printf("Reading wav header...\n\n");
	file->ReadHeader(file->instance);
	file->PrintInfo(file->instance);
	printf("Closing...\n");
	apiPtr->Close(apiPtr->instance);

	printf("Destroying...\n");
	apiPtr->Destroy(apiPtr->instance);
	free(apiPtr);
	file->Destroy(file->instance);
	free(file);

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