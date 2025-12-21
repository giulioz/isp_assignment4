#include "bmpRepo.h"

#include <stdio.h>

BmpRepo* bmpRepo_init(void) {
  BmpRepo* repo = (BmpRepo*)malloc(sizeof(BmpRepo));
  if (repo != NULL) {
    repo->lastBmpId = 0;
    repo->entries = NULL;
  }
  return repo;
}

void bmpRepo_free(BmpRepo* repo) {
  if (repo != NULL) {
    if (repo->entries != NULL) {
      for (int i = 0; i < repo->lastBmpId; ++i) {
        if (repo->entries[i].pixelData != NULL) {
          free(repo->entries[i].pixelData);
        }
      }
      free(repo->entries);
    }
    free(repo);
  }
}

BmpRepoEntry* bmpRepo_addEmptyBmp(BmpRepo* repo, size_t width, size_t height) {
  if (repo == NULL) {
    return NULL;
  }

  int newLastBmpId = repo->lastBmpId + 1;
  BmpRepoEntry* newEntries = (BmpRepoEntry*)realloc(
      repo->entries, (newLastBmpId) * sizeof(BmpRepoEntry));
  if (newEntries == NULL) {
    return NULL;
  }

  repo->entries = newEntries;
  BmpRepoEntry* newEntry = &repo->entries[repo->lastBmpId];
  newEntry->bmpId = repo->lastBmpId;
  newEntry->width = width;
  newEntry->height = height;
  newEntry->pixelData =
      (uint8_t*)calloc(width * height * BMP_STRIDE, sizeof(uint8_t));
  if (newEntry->pixelData == NULL) {
    return NULL;
  }

  repo->lastBmpId = newLastBmpId;
  return newEntry;
}

BmpRepoEntry* bmpRepo_loadFromFile(BmpRepo* repo, const char* filename) {
  if (repo == NULL) {
    return NULL;
  }

  BmpHeader header;

  FILE* file = fopen(filename, "rb");
  if (file == NULL) {
    return NULL;
  }

  if (fread(&header, sizeof(BmpHeader), 1, file) != 1) {
    fclose(file);
    return NULL;
  }

  int isValidBmp =
      (header.b_ == 'B' && header.m_ == 'M' &&
       header.number_of_bits_per_pixel_ == 32 && header.compression_ == 3);
  if (!isValidBmp) {
    fclose(file);
    return NULL;
  }

  BmpRepoEntry* newEntry =
      bmpRepo_addEmptyBmp(repo, header.width_, header.height_);
  if (newEntry == NULL) {
    fclose(file);
    return NULL;
  }

  fseek(file, header.offset_pixel_array_, SEEK_SET);
  fread(newEntry->pixelData, header.raw_bitmap_data_size_, 1, file);
  fclose(file);

  return newEntry;
}
