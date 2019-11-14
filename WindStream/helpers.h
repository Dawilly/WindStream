#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"

#ifndef __INCLUDED__HELPERS__
#define __INCLUDED__HELPERS__

void* Cmalloc(size_t, char*, bool);
void  ScanForIntInRange(FILE*, int*, int, int);

#endif 
