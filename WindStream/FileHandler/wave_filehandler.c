#include <stdint.h>
#include "wave_filehandler.h"
#include "../helpers.h"
#include <math.h>

typedef struct riff_chunk {
	byte ChunkId[5];
	uint32_t ChunkSize;
	byte Format[5];
} RiffChunk;

typedef struct fmt_chunk {
	byte Subchunk1ID[5];
	uint32_t Subchunk1Size;
	uint16_t AudioFormat;
	uint16_t NumChannels;
	uint32_t SampleRate;
	uint32_t ByteRate;
	uint16_t BlockAlign;
	uint16_t BitsPerSample;
	// Extension Data goes here
} FmtChunk;

typedef struct data_chunk {
	byte Subchunk2ID[5];
	uint32_t Subchunk2Size;
} DataChunk;

typedef struct wave_header {
	RiffChunk* riff;
	FmtChunk* fmt;
	DataChunk* data;
} WaveHeader;

typedef struct wave_file {
	char* fileName;
	FILE* fp;
	byte fileState;
	WaveHeader* header;

	unsigned long totalSamples;
	// Data array may need to be bytes, not floats.
	float* data;
	void* wArray;
	long wArrayLength;
	long startingPosOfFile;
	long decodedCount;
	long pointOnDataArray;
	
	void (*ReadSamples)(WaveFile*, long);
} WaveFile;

void wave_ReadSamplesSize8(WaveFile*, long);
void wave_ReadSamplesSize16(WaveFile*, long);
void wave_ReadSamplesSize32(WaveFile*, long);
bool wave_ReadRiffChunk(WaveFile*);
int  wave_ReadFmtChunk(WaveFile*);
bool wave_ReadDataChunk(WaveFile*);
void ReadCString(FILE*, char*, int);

WaveFile* Wave_CreateInstance() {
	WaveFile* newFile = Cmalloc(sizeof(WaveFile), "WaveFile.File", FALSE);
	if (!newFile) return NULL;

	newFile->fp = NULL;
	newFile->fileState = UNSET;
	newFile->header = Cmalloc(sizeof(WaveHeader), "WaveFile.Header", FALSE);

	if (newFile->header == NULL) {
		// Error
		free(newFile);
		return NULL;
	}

	newFile->header->riff = malloc(sizeof(RiffChunk));
	newFile->header->fmt = malloc(sizeof(FmtChunk));
	newFile->header->data = malloc(sizeof(DataChunk));

	// TO-DO: Check for errors

	return newFile;
}

void Wave_OpenFile(WaveFile* ptr, const char* filename) {
	if (filename == NULL) {
		//Print error
		return;
	}

	ptr->fp = fopen(filename, "r");

	if (ptr->fp == NULL) {
		//Print error
		return;
	}

	size_t byteCount = sizeof(char) * (strlen(filename) + 1);
	ptr->fileState = OPEN;
	ptr->fileName = Cmalloc(byteCount, "Wave.OpenFile", TRUE);
	strcpy(ptr->fileName, filename);
}

void Wave_CloseFile(WaveFile* ptr) {
	if (fclose(ptr->fp) == 0) {
		ptr->fileState = CLOSED;
	}
	return;
}

void Wave_ReadHeader(WaveFile* ptr) {
	if (ptr->fileState != OPEN) return;

	if (wave_ReadRiffChunk(ptr) == FALSE) return;

	if (wave_ReadFmtChunk(ptr) == FALSE) return;

	// This is temp. For now the focus is SubChunk1Size = 16
	if (ptr->header->fmt->Subchunk1Size != 16) return;

	if (wave_ReadDataChunk(ptr) == FALSE) return;

	return;
}

void Wave_SetupInstance(WaveFile* ptr, int milliseconds) {
	FmtChunk* fmt = ptr->header->fmt;
	DataChunk* data = ptr->header->data;
	size_t byteCount;

	ptr->totalSamples = data->Subchunk2Size / (fmt->BitsPerSample / 8);
	ptr->data = malloc(sizeof(float) * ptr->totalSamples);
	ptr->startingPosOfFile = ftell(ptr->fp);
	ptr->decodedCount = 0;
	ptr->pointOnDataArray = 0;

	double seconds = milliseconds / 1000.0;
	ptr->wArrayLength = floor(seconds * ptr->header->fmt->SampleRate);

	if (fmt->BitsPerSample == 8) {
		byteCount = sizeof(int8_t) * ptr->wArrayLength;
		ptr->ReadSamples = wave_ReadSamplesSize8;
	} else if (fmt->BitsPerSample == 16) {
		byteCount = sizeof(short) * ptr->wArrayLength;
		ptr->ReadSamples = wave_ReadSamplesSize16;
	} else if (fmt->BitsPerSample == 32) {
		byteCount = sizeof(float) * ptr->wArrayLength;
		ptr->ReadSamples = wave_ReadSamplesSize32;
	} else {
		// Error out.
		return;
	}

	ptr->wArray = Cmalloc(sizeof(byteCount), "Wave.Setup", TRUE);
	return;
}

void Wave_ReadSamples(WaveFile* ptr) {
	long count = min(ptr->totalSamples - ptr->decodedCount, ptr->wArrayLength);
	ptr->ReadSamples(ptr, count);
}

float* Wave_GetDataBuffer(WaveFile* ptr) {
	return ptr->data;
}

unsigned long Wave_GetTotalSamples(WaveFile* ptr) {
	return ptr->totalSamples;
}

void Wave_ResetPosition(WaveFile* ptr) {
	if (fseek(ptr->fp, ptr->startingPosOfFile, SEEK_SET) == 0) {
		ptr->decodedCount = 0;
		ptr->pointOnDataArray = 0;
	} else {
		fprintf(stderr, "Unable to reset file position for: %s\n", ptr->fileName);
	}
	return;
}

void Wave_DestroyInstance(WaveFile* ptr) {
	if (ptr->fp != NULL) {
		fclose(ptr->fp);
		ptr->fp = NULL;
	}

	DataChunk* data = ptr->header->data;
	FmtChunk* fmt = ptr->header->fmt;
	RiffChunk* riff = ptr->header->riff;

	free(ptr->fileName);
	free(ptr->data);
	free(ptr->wArray);

	free(riff->ChunkId);
	free(riff->Format);
	free(riff);

	free(fmt->Subchunk1ID);
	free(fmt);
	
	free(data->Subchunk2ID);
	free(data);
	
	free(ptr->header);
	free(ptr);
}

void Wave_PrintInfo(WaveFile* ptr) {
	RiffChunk* riff = ptr->header->riff;
	FmtChunk* fmt = ptr->header->fmt;

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

void wave_ReadSamplesSize8(WaveFile* ptr, long count) {
	int8_t* wArray = ptr->wArray;

	// May need to take lock, doc.
	fread(wArray, sizeof(int8_t), count, ptr->fp);

	for (long i = 0; i < count; i++) {
		ptr->data[ptr->pointOnDataArray + i] = (wArray[i] / (float)SIGNEDBYTEMAX);
	}

	ptr->pointOnDataArray += count;

	return;
}

void wave_ReadSamplesSize16(WaveFile* ptr, long count) {
	short* wArray = ptr->wArray;
	
	// May need to take lock, doc.
	fread(wArray, sizeof(int16_t), count, ptr->fp);

	for (long i = 0; i < count; i++) {
		ptr->data[ptr->pointOnDataArray + i] = (wArray[i] / (float)SIGNEDSHORTMAX);
	}

	ptr->pointOnDataArray += count;

	return;
}

void wave_ReadSamplesSize32(WaveFile* ptr, long count) {
	float* wArray = ptr->wArray;

	fread(wArray, sizeof(float), count, ptr->fp);
	
	ptr->pointOnDataArray += count;

	return;
}

bool wave_ReadRiffChunk(WaveFile* ptr) {
	RiffChunk* riff = ptr->header->riff;

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

bool wave_ReadFmtChunk(WaveFile* ptr) {
	FmtChunk* fmt = ptr->header->fmt;

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

bool wave_ReadDataChunk(WaveFile* ptr) {
	DataChunk* data = ptr->header->data;
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
