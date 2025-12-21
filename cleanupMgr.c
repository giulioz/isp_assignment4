#include "cleanupMgr.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

CleanupMgr* cleanupMgr_init(void) {
  CleanupMgr* mgr = (CleanupMgr*)malloc(sizeof(CleanupMgr));
  if (mgr != NULL) {
    mgr->ptrs = NULL;
    mgr->nPtrs = 0;
  } else {
    printf("[ERROR] Memory allocation failed!\n\n");
  }
  return mgr;
}

void cleanupMgr_cleanupAll(CleanupMgr* mgr) {
  assert(mgr != NULL);

  for (int i = 0; i < mgr->nPtrs; ++i) {
    if (mgr->ptrs[i] != NULL) {
      free(mgr->ptrs[i]);
    }
  }
  if (mgr->ptrs != NULL) {
    free(mgr->ptrs);
  }
  free(mgr);
}

void cleanupMgr_addPtr(CleanupMgr* mgr, void* ptr) {
  assert(mgr != NULL);

  void** newPtrs = (void**)realloc(mgr->ptrs, (mgr->nPtrs + 1) * sizeof(void*));
  if (newPtrs == NULL) {
    printf("[ERROR] Memory allocation failed!\n\n");
    return;
  }

  mgr->ptrs = newPtrs;
  mgr->ptrs[mgr->nPtrs] = ptr;
  mgr->nPtrs++;
}

void cleanupMgr_replacePtr(CleanupMgr* mgr, void* oldPtr, void* newPtr) {
  assert(mgr != NULL);

  if (oldPtr == NULL) {
    cleanupMgr_addPtr(mgr, newPtr);
    return;
  }

  for (int i = 0; i < mgr->nPtrs; ++i) {
    if (mgr->ptrs[i] == oldPtr) {
      mgr->ptrs[i] = newPtr;
      return;
    }
  }

  // Not found, add new one
  cleanupMgr_addPtr(mgr, newPtr);
}

void* cleanupMgr_freeSingle(CleanupMgr* mgr, void* ptr) {
  assert(mgr != NULL);

  for (int i = 0; i < mgr->nPtrs; ++i) {
    if (mgr->ptrs[i] == ptr) {
      free(mgr->ptrs[i]);
      mgr->ptrs[i] = NULL;
    }
  }

  // TODO: compact the ptrs array

  return NULL;  // useful to set the pointer to NULL in the caller
}
