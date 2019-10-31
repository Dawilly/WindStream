#include "../definitions.h"
#include <stdint.h>

#ifndef __WAVFILESTRUCTURE__INCLUDED__
#define __WAVFILESTRUCTURE__INCLUDED__

typedef struct riff_chunk {
	byte ChunkId[5];
	uint32_t ChunkSize;
	byte Format[5];
} RIFFCHUNK;

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
} FMTCHUNK;

typedef struct wav_header {
	RIFFCHUNK* riff;
	FMTCHUNK* fmt;
	DATACHUNK* data;
} WAVHEADER;

typedef struct data_chunk {
	byte Subchunk2ID[5];
	uint32_t Subchunk2Size;
} DATACHUNK;

#endif // !__WAVFILESTRUCTURE__INCLUDED__