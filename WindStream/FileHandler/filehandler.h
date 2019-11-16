#include <stdio.h>

#ifndef __FILEHANDLER__INCLUDED__
#define __FILEHANDLER__INCLUDED__

typedef struct file_handler {
	void* instance;
	void* (*Create)();
	void  (*Open)(void*, const char*);
	void  (*Close)(void*);
	void  (*Destroy)(void*);
	void  (*Initialize)(void*);
	void  (*ReadHeader)(void*);
	void  (*PrintInfo)(void*);
	void  (*Reset)(void*);
	
	float* (*Get_Buffer)(void*);
	unsigned long (*Get_TotalSamples)(void*);
} FileHandler;

#define UNSET 0x00
#define CLOSED 0x01
#define OPEN 0x02
#define END 0x03

#endif // !__FILEHANDLER__INCLUDED__
