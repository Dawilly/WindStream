#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../definitions.h"
#include "filehandler.h"

#ifndef __WAVFILEHANDLER__INCLUDED__
#define __WAVFILEHANDLER__INCLUDED__

typedef struct wave_file WaveFile;

WaveFile* Wave_CreateInstance();
void Wave_OpenFile(WaveFile*, char*);
void Wave_ReadHeader(WaveFile*);
void Wave_SetupInstance(WaveFile*);
void Wave_PrintInfo(WaveFile*);

#endif // !__WAVFILEHANDLER__INCLUDED__
