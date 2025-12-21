#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

#include "bmp.h"
#include "cleanupMgr.h"

char* readStringAlloc(CleanupMgr* mgr);

char* getNWord(const char* str, int n, CleanupMgr* mgr);

FILE* bmpReadHeader(const char* filename, BmpHeader* header);

#endif  // COMMON_H
