#include "audiohandler.h"

typedef struct audioHandler {
	void (*Open)();
	void (*Close)();

} AUDIOHANDLER;