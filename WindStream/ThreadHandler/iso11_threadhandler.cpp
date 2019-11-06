#include "iso11_threadhandler.h"

typedef struct iso11_thread {
	std::thread instance;
} ISOTHREAD;

ISOTHREAD* CreateIsoThread() {
	ISOTHREAD* newThread = new ISOTHREAD;

	return newThread;
}