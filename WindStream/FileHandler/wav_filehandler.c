#include "wav_filehandler.h"
#include "wav_filestructure.h"

typedef struct wav_file {
	FILE* fp;
	byte fileState;
	WAVHEADER* header;
} WAVFILE;



WAVFILE* CreateWavFile() {
	WAVFILE* newFile = malloc(sizeof(WAVFILE));
	if (newFile == NULL) {
		//Error
		return NULL;
	}

	newFile->fp = NULL;
	newFile->fileState = UNSET;
	newFile->header = malloc(sizeof(WAVHEADER));

	if (newFile->header == NULL) {
		// Error
		free(newFile);
		return NULL;
	}

	newFile->header->riff = malloc(sizeof(RIFFCHUNK));
	newFile->header->fmt = malloc(sizeof(FMTCHUNK));

	return newFile;
}

void OpenWavFile(WAVFILE* ptr, char* filename) {
	if (filename == NULL) {
		//Print error
		return;
	}

	ptr->fp = fopen(filename, "r");

	if (ptr->fp == NULL) {
		//Print error
		return;
	}

	ptr->fileState = OPEN;
}

void ReadWavHeader(WAVFILE* ptr) {
	if (ptr->fileState != OPEN) return;

	size_t rc;
	RIFFCHUNK* riff = ptr->header->riff;
	FMTCHUNK* fmt = ptr->header->fmt;

	rc = fread(riff, sizeof(RIFFCHUNK), 1, ptr->fp);
	if (rc != 1 || strcmp("RIFF", riff->ChunkId)) return;

	rc = fread(fmt->Subchunk1ID, sizeof(byte), 4, ptr->fp);
	if (rc != 4 || strcmp("fmt ", fmt->Subchunk1ID)) return;

	fread(fmt->Subchunk1Size, sizeof(int), 1, ptr->fp);

	switch (fmt->Subchunk1Size) {
		case 16:
			fmt->details = malloc(sizeof(FMTDETAILCHUNK16));
			if (fmt->details == 0) {
				// Error out
				return;
			}
			fread(fmt->details, sizeof(FMTDETAILCHUNK16), 1, ptr->fp);
			break;
		case 18:
			break;
		case 40:
			break;
		default:
			// Error out
			break;
	}

	return;
}

void ReadWavSamples(WAVFILE* ptr) {

}
//size_t fread(void* ptr, size_t size_of_elements, size_t number_of_elements, FILE* a_file);

//size_t fwrite(const void* ptr, size_t size_of_elements, size_t number_of_elements, FILE* a_file);