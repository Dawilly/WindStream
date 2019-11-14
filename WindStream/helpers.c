#include "helpers.h"

void printError(FILE*);

void* Cmalloc(size_t bytes, char* caller, bool isRequired) {
	void* results = malloc(bytes);
	if (results == 0) {
		fprintf(stderr, "Unable to allocate memory.\nByte count: %llu\tCaller: %s\n", bytes, caller);
		if (isRequired) exit(-1);
	}

	return results;
}

void ScanForIntInRange(FILE* fp, int* ref, int lowerLimit, int upperLimit) {
	int results = 0;

	do {
		fscanf(fp, "%d", &results);
		if (results < lowerLimit || results > upperLimit) printError(stdin);
	} while (results < lowerLimit || results > upperLimit);

	*ref = results;
	return;
}

void printError(FILE* fp) {
	fprintf(fp, "Invalid input. Try again.\n");
}