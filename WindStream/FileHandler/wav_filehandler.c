#include "wav_filestructure.h"
#include "wav_filehandler.h"

typedef struct wav_file {
	char* fileName;
	FILE* fp;
	byte fileState;
	WAVHEADER* header;

	unsigned long totalSamples;
	float* data;
	void* wArray;
	long wArrayLength;
	long startingPosOfFile;
	long decodedCount;
	long pointOnDataArray;
	
	void (*ReadSamples)(WAVFILE*, long);
} WAVFILE;

void ReadSamplesSize8(WAVFILE*, long);
void ReadSamplesSize16(WAVFILE*, long);
void ReadSamplesSize32(WAVFILE*, long);
bool ReadRiffChunk(WAVFILE*);
int ReadFmtChunk(WAVFILE*);
bool ReadDataChunk(WAVFILE*);
void ReadCString(FILE*, char*, int);

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
	newFile->header->data = malloc(sizeof(DATACHUNK));

	// TO-DO: Check for errors

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
	ptr->fileName = malloc(sizeof(char) * (strlen(filename) + 1));
	strcpy(ptr->fileName, filename);
}

void ReadWavHeader(WAVFILE* ptr) {
	if (ptr->fileState != OPEN) return;

	if (ReadRiffChunk(ptr) == FALSE) return;

	if (ReadFmtChunk(ptr) == FALSE) return;

	// This is temp. For now the focus is SubChunk1Size = 16
	if (ptr->header->fmt->Subchunk1Size != 16) return;

	if (ReadDataChunk(ptr) == FALSE) return;

	return;
}

void SetupWavFile(WAVFILE* ptr) {
	FMTCHUNK* fmt = ptr->header->fmt;
	DATACHUNK* data = ptr->header->data;

	ptr->totalSamples = data->Subchunk2Size / (fmt->BitsPerSample / 8);
	ptr->data = malloc(sizeof(float) * ptr->totalSamples);
	ptr->startingPosOfFile = ftell(ptr->fp);
	ptr->decodedCount = 0;

	if (fmt->BitsPerSample == 8) {
		//ptr->wArray = malloc
		//ptr->ReadSamples = ReadSamplesSize8;
	}
	else if (fmt->BitsPerSample == 16) {
		ptr->ReadSamples = ReadSamplesSize16;
	}
	else if (fmt->BitsPerSample == 32) {
		ptr->ReadSamples = ReadSamplesSize32;
	}
	else {
		// Error out.
		return;
	}
}

void ReadWavSamples(WAVFILE* ptr) {
	long count = min(ptr->totalSamples - ptr->decodedCount, ptr->wArrayLength);
	ptr->ReadSamples(ptr, count);
}

void PrintWavFileInfo(WAVFILE* ptr) {
	RIFFCHUNK* riff = ptr->header->riff;
	FMTCHUNK* fmt = ptr->header->fmt;

	printf("\nFile:\n");
	printf("File Name: %s\n", ptr->fileName);
	printf("File State: %d\n", ptr->fileState);

	printf("\nRIFF Chunk:\n");
	printf("Chunk Id: %s\n", riff->ChunkId);
	printf("Chunk Size: %d\n", riff->ChunkSize);
	printf("Format: %s\n", riff->Format);

	printf("\nFMT Chunk:\n");
	printf("Sub Chunk 1 Id: %s\n", fmt->Subchunk1ID);
	printf("Sub Chunk 1 Size: %d\n", fmt->Subchunk1Size);
	printf("Audio Format: %d\n", fmt->AudioFormat);
	printf("Number of Channels: %d\n", fmt->NumChannels);
	printf("Sample Rate: %d\n", fmt->SampleRate);
	printf("Byte Rate: %d\n", fmt->ByteRate);
	printf("Block Align: %d\n", fmt->BlockAlign);
	printf("Bits Per Sample: %d\n", fmt->BitsPerSample);
	// Extension Data goes here
}

/////////////////////////
/// Private Functions ///
/////////////////////////

void ReadSamplesSize8(WAVFILE* ptr, long count) {
	
}

void ReadSamplesSize16(WAVFILE* ptr, long count) {
	short* wArray = ptr->wArray;
	
	// May need to take lock, doc.
	fread(wArray, sizeof(int16_t), count, ptr->fp);

	for (long i = 0; i < count; i++) {
		ptr->data[ptr->pointOnDataArray + i] = (wArray[i] / (float)SIGNEDSHORTMAX);
	}

	return;
}

void ReadSamplesSize32(WAVFILE* ptr, long count) {
	float* wArray = ptr->wArray;

	fread(wArray, sizeof(float), count, ptr->fp);
	
	return;
}

bool ReadRiffChunk(WAVFILE* ptr) {
	RIFFCHUNK* riff = ptr->header->riff;

	ReadCString(ptr->fp, riff->ChunkId, 4);
	if (strcmp("RIFF", riff->ChunkId) != 0) {
		printf("Error: ChunkID is not RIFF.\nChunkID: %s\n", riff->ChunkId);
		return FALSE;
	}

	fread(&riff->ChunkSize, sizeof(int), 1, ptr->fp);

	ReadCString(ptr->fp, riff->Format, 4);
	if (strcmp("WAVE", riff->Format) != 0) {
		printf("Error: Format is not WAVE.\nFormat: %s\n", riff->Format);
		return FALSE;
	}

	return TRUE;
}

bool ReadFmtChunk(WAVFILE* ptr) {
	FMTCHUNK* fmt = ptr->header->fmt;

	ReadCString(ptr->fp, fmt->Subchunk1ID, 4);
	if (strcmp("fmt ", fmt->Subchunk1ID) != 0) {
		printf("Error: SubChunk1ID is not fmt .\nSubChunk1ID: %s\n", fmt->Subchunk1ID);
		return FALSE;
	}

	fread(&fmt->Subchunk1Size, sizeof(uint32_t), 1, ptr->fp);
	fread(&fmt->AudioFormat, sizeof(uint16_t), 1, ptr->fp);
	fread(&fmt->NumChannels, sizeof(uint16_t), 1, ptr->fp);
	fread(&fmt->SampleRate, sizeof(uint32_t), 1, ptr->fp);
	fread(&fmt->ByteRate, sizeof(uint32_t), 1, ptr->fp);
	fread(&fmt->BlockAlign, sizeof(uint16_t), 1, ptr->fp);
	fread(&fmt->BitsPerSample, sizeof(uint32_t), 1, ptr->fp);

	return TRUE;
}

bool ReadDataChunk(WAVFILE* ptr) {
	DATACHUNK* data = ptr->header->data;
	ReadCString(ptr->fp, data->Subchunk2ID, 4);
	if (strcmp("data", data->Subchunk2ID) != 0) {
		printf("Error: Subchunk2ID is not data.\nSubchunk2ID: %s\n", data->Subchunk2ID);
		return FALSE;
	}
	// To-Do: Add malloc check

	return TRUE;
}

void ReadCString(FILE* fp, char* str, int size) {
	fread(str, sizeof(char), 4, fp);
	str[size] = '\0';
}
