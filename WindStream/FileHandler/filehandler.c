#include "filehandler.h"

typedef struct audioHandler {
	void (*Open)();
	void (*Close)();
} AUDIOHANDLER;

void OpenAudioDevice(AUDIOHANDLER* ah) {

}