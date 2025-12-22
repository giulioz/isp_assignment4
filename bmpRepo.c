#include "bmpRepo.h"

#include <assert.h>
#include <stdio.h>

#include "strings.h"

BmpRepo* bmpRepo_init(MemoryMgr* mgr) {
  assert(mgr != NULL);

  BmpRepo* repo = (BmpRepo*)memoryMgr_malloc(mgr, sizeof(BmpRepo));
  if (repo != NULL) {
    repo->lastBmpId = 0;
    repo->entries = memoryMgr_malloc(mgr, 0);  // Start with zero entries
    repo->mgr = mgr;
  }
  return repo;
}

BmpRepoEntry* bmpRepo_createEmptyBmp(BmpRepo* repo, int width, int height) {
  assert(repo != NULL);

  // Add space for a new entry
  int newLastBmpId = repo->lastBmpId + 1;
  BmpRepoEntry** newEntries = (BmpRepoEntry**)memoryMgr_realloc(
      repo->mgr, repo->entries, newLastBmpId * sizeof(BmpRepoEntry*));
  if (newEntries == NULL) {
    return NULL;
  }
  repo->entries = newEntries;

  // Array of pointers, see comment in BmpRepo struct
  BmpRepoEntry* newEntry =
      (BmpRepoEntry*)memoryMgr_malloc(repo->mgr, sizeof(BmpRepoEntry));
  newEntry->bmpId = repo->lastBmpId;
  newEntry->width = width;
  newEntry->height = height;
  newEntry->pixelData = (uint8_t*)memoryMgr_malloc(
      repo->mgr, width * height * BMP_STRIDE * sizeof(uint8_t));
  if (newEntry->pixelData == NULL) {
    printf(ERROR_MEMALLOC);
    return NULL;
  }

  repo->entries[repo->lastBmpId] = newEntry;
  repo->lastBmpId = newLastBmpId;
  return newEntry;
}

BmpRepoEntry* bmpRepo_getBmpById(BmpRepo* repo, int bmpId) {
  assert(repo != NULL);

  for (int i = 0; i < repo->lastBmpId; i++) {
    if (repo->entries[i]->bmpId == bmpId) {
      return repo->entries[i];
    }
  }
  return NULL;
}
