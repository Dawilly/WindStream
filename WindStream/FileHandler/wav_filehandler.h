#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../definitions.h"
#include "filehandler.h"

#ifndef __WAVFILEHANDLER__INCLUDED__
#define __WAVFILEHANDLER__INCLUDED__

typedef struct wav_file WAVFILE;

WAVFILE* CreateWavFile();
void OpenWavFile(WAVFILE*, char*);
void ReadWavHeader(WAVFILE*);
void SetupWavFile(WAVFILE*);
void PrintWavFileInfo(WAVFILE*);

#endif // !__WAVFILEHANDLER__INCLUDED__
