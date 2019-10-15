#include "audiohandler.h"

typedef struct audioHandler {
	void (*Open)();
	void (*Close)();
} AUDIOHANDLER;

void OpenDevice(AUDIOHANDLER* ah) {

}