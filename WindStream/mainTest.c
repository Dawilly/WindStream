#include "WindStream.h"
#include "openal_apihandler.h"

int main() {
	printf("Test...\n");

	OAL_API* test = CreateOalApi(GetLibrary, GetFunctionAddresses);

	return 0;
}