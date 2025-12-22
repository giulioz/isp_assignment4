#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

#include "bmp.h"
#include "memoryMgr.h"

char* readStringAlloc(MemoryMgr* mgr);

char* getNWord(const char* str, int n, MemoryMgr* mgr);

FILE* bmpReadHeader(const char* filename, BmpHeader* header);

FILE* bmpWriteHeader(const char* filename, BmpHeader* header, int width, int height);

#endif  // COMMON_H
