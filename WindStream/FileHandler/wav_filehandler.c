#include "wav_filehandler.h"
#include "wav_filestructure.h"

typedef struct wav_file {
	char* fileName;
	FILE* fp;
	byte fileState;
	WAVHEADER* header;
} WAVFILE;

bool ReadRiffChunk(WAVFILE*);
int ReadFmtTopChunk(WAVFILE*);
void ReadCString(FILE*, char*, int);
void PrintWavFileInfo(WAVFILE*);
void PrintDetails16(FMTDETAILCHUNK16*);

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
	ptr->fileName = malloc(sizeof(char) * (strlen(filename) + 1));
	strcpy(ptr->fileName, filename);
}

void ReadWavHeader(WAVFILE* ptr) {
	if (ptr->fileState != OPEN) return;

	FMTCHUNK* fmt = ptr->header->fmt;

	if (ReadRiffChunk(ptr) == FALSE) return;

	if (ReadFmtTopChunk(ptr) == 0) {
		printf("Unable to read a support SubChunk1Size.\nSubChunk1Size: %d\n", fmt->Subchunk1Size);
		return;
	}

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

	PrintWavFileInfo(ptr);

	return;
}

void ReadWavSamples(WAVFILE* ptr) {

}

void PrintWavFileInfo(WAVFILE* ptr) {
	printf("File Name: %s\n", ptr->fileName);
	printf("File State: %d\n", ptr->fileState);
	printf("Chunk Size: %d\n", ptr->header->riff->ChunkSize);
	printf("Sub Chunk 1 Size: %d\n", ptr->header->fmt->Subchunk1Size);

	switch (ptr->header->fmt->Subchunk1Size) {
		case 16:
			PrintDetails16(ptr->header->fmt->details);
			break;
		case 18:
			break;
		case 40:
			break;
		default:
			break;
	}
	return;
}

void PrintDetails16(FMTDETAILCHUNK16* details) {
	printf("Audio Format: %d\n", details->AudioFormat);
	printf("Number of Channels: %d\n", details->NumChannels);
	printf("Sample Rate: %d\n", details->SampleRate);
	printf("Byte Rate: %d\n", details->ByteRate);
	printf("Block Align: %d\n", details->BlockAlign);
	printf("Bits Per Sample: %d\n", details->BitsPerSample);
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

int ReadFmtTopChunk(WAVFILE* ptr) {
	FMTCHUNK* fmt = ptr->header->fmt;

	ReadCString(ptr->fp, fmt->Subchunk1ID, 4);
	if (strcmp("fmt ", fmt->Subchunk1ID) != 0) {
		printf("Error: SubChunk1ID is not fmt .\nSubChunk1ID: %s\n", fmt->Subchunk1ID);
		return 0;
	}

	return (fread(&fmt->Subchunk1Size, sizeof(int), 1, ptr->fp)) == 0 ? 0 : fmt->Subchunk1Size;
}

void ReadCString(FILE* fp, char* str, int size) {
	fread(str, sizeof(char), 4, fp);
	str[size] = '\0';
}
//size_t fread(void* ptr, size_t size_of_elements, size_t number_of_elements, FILE* a_file);

//size_t fwrite(const void* ptr, size_t size_of_elements, size_t number_of_elements, FILE* a_file);