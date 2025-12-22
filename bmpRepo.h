#ifndef BMP_REPO_H
#define BMP_REPO_H

#include <stdint.h>
#include <stdlib.h>

#include "bmp.h"
#include "memoryMgr.h"

#define BMP_STRIDE 4

typedef struct BmpRepoEntry {
  int bmpId;
  uint8_t* pixelData;
  int width;
  int height;

  // TODO: support cropping without copying data
} BmpRepoEntry;

typedef struct BmpRepo {
  int lastBmpId;

  // We use a dynamic array of pointers to make reallocations easier
  // We used a dynamic array of structs before, but that would cause problems if
  // createEmptyBmp is called after getBmpById, as the pointer returned by
  // getBmpById could be invalidated by a realloc in createEmptyBmp.
  BmpRepoEntry** entries;

  // We keep the MemoryMgr here to make memory management streamlined
  MemoryMgr* mgr;
} BmpRepo;

// Returns NULL if memory allocation failed
BmpRepo* bmpRepo_init(MemoryMgr* mgr);

// Returns NULL if memory allocation failed
BmpRepoEntry* bmpRepo_createEmptyBmp(BmpRepo* repo, int width, int height);

BmpRepoEntry* bmpRepo_getBmpById(BmpRepo* repo, int bmpId);

#endif  // BMP_REPO_H
