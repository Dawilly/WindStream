#include "../definitions.h"

#ifndef __WAVFILESTRUCTURE__INCLUDED__
#define __WAVFILESTRUCTURE__INCLUDED__

typedef struct riff_chunk {
	byte ChunkId[5];
	int ChunkSize;
	byte Format[5];
} RIFFCHUNK;

typedef struct fmt_chunk {
	byte Subchunk1ID[5];
	int Subchunk1Size;
	void* details;
} FMTCHUNK;

typedef struct detail_chunk_16 {
	short AudioFormat;
	short NumChannels;
	int SampleRate;
	int ByteRate;
	short BlockAlign;
	short BitsPerSample;
} FMTDETAILCHUNK16;

typedef struct wav_header {
	RIFFCHUNK* riff;
	FMTCHUNK* fmt;
} WAVHEADER;

typedef struct wav_data {
	byte Subchunk2ID[5];
	unsigned int Subchunk2Size;

} WAVDATA;

#endif // !__WAVFILESTRUCTURE__INCLUDED__