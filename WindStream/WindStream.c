// WindStream.c : Defines the entry point for the application.
//

#include "WindStream.h"

int main() {
	void* fp;
	void* ptr = LoadLibrary("OpenAL32.dll");

	if (ptr != NULL) fp = GetProcAddress(ptr, "alGenBuffers");

	return 0;
}

void GetFunctionAddresses() {

}