#include "bmpRepo.h"

#include <assert.h>
#include <stdio.h>

#include "strings.h"

BmpRepo* bmpRepo_init(CleanupMgr* mgr) {
  assert(mgr != NULL);

  BmpRepo* repo = (BmpRepo*)malloc(sizeof(BmpRepo));
  if (repo != NULL) {
    repo->lastBmpId = 0;
    repo->entries = malloc(0);  // Start with zero entries
    repo->mgr = mgr;
    cleanupMgr_addPtr(mgr, repo);
    cleanupMgr_addPtr(mgr, repo->entries);
  } else {
    printf(ERROR_MEMALLOC);
  }
  return repo;
}

BmpRepoEntry* bmpRepo_createEmptyBmp(BmpRepo* repo, int width, int height) {
  assert(repo != NULL);

  // Add space for a new entry
  int newLastBmpId = repo->lastBmpId + 1;
  BmpRepoEntry** newEntries = (BmpRepoEntry**)realloc(
      repo->entries, newLastBmpId * sizeof(BmpRepoEntry*));
  if (newEntries == NULL) {
    printf(ERROR_MEMALLOC);
    return NULL;
  }
  cleanupMgr_replacePtr(repo->mgr, repo->entries, newEntries);
  repo->entries = newEntries;

  // Array of pointers, see comment in BmpRepo struct
  BmpRepoEntry* newEntry = (BmpRepoEntry*)malloc(sizeof(BmpRepoEntry));
  newEntry->bmpId = repo->lastBmpId;
  newEntry->width = width;
  newEntry->height = height;
  newEntry->pixelData =
      (uint8_t*)calloc(width * height * BMP_STRIDE, sizeof(uint8_t));
  if (newEntry->pixelData == NULL) {
    printf(ERROR_MEMALLOC);
    return NULL;
  }
  cleanupMgr_addPtr(repo->mgr, newEntry->pixelData);
  cleanupMgr_addPtr(repo->mgr, newEntry);

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
