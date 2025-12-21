#ifndef BMP_REPO_H
#define BMP_REPO_H

#include <stdint.h>
#include <stdlib.h>
#include "bmp.h"

#define BMP_STRIDE 4

typedef struct BmpRepoEntry {
  int bmpId;
  uint8_t *pixelData;
  size_t width;
  size_t height;
} BmpRepoEntry;

typedef struct BmpRepo {
  int lastBmpId;
  BmpRepoEntry *entries;
} BmpRepo;

// Returns NULL if failed
BmpRepo* bmpRepo_init(void);

void bmpRepo_free(BmpRepo* repo);

// Returns NULL if failed
BmpRepoEntry* bmpRepo_addEmptyBmp(BmpRepo* repo, size_t width, size_t height);

// Returns NULL if failed
BmpRepoEntry* bmpRepo_loadFromFile(BmpRepo* repo, const char* filename);

#endif // BMP_REPO_H
