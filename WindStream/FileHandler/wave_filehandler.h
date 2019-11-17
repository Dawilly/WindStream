#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../definitions.h"
#include "filehandler.h"

#ifndef __WAVFILEHANDLER__INCLUDED__
#define __WAVFILEHANDLER__INCLUDED__

typedef struct wave_file WaveFile;

WaveFile* Wave_CreateInstance();
void Wave_OpenFile(WaveFile*, const char*);
void Wave_CloseFile(WaveFile*);
void Wave_ReadHeader(WaveFile*);
void Wave_SetupInstance(WaveFile*);
void Wave_PrintInfo(WaveFile*);
float* Wave_GetDataBuffer(WaveFile*);
unsigned long Wave_GetTotalSamples(WaveFile*);
void Wave_ResetPosition(WaveFile*);
void Wave_DestroyInstance(WaveFile*);

#endif // !__WAVFILEHANDLER__INCLUDED__
