#include "memoryMgr.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strings.h"

MemoryMgr* memoryMgr_init(void) {
  MemoryMgr* mgr = (MemoryMgr*)malloc(sizeof(MemoryMgr));
  if (mgr != NULL) {
    mgr->ptrs = NULL;
    mgr->nPtrs = 0;
  } else {
    printf(ERROR_MEMALLOC);
  }
  return mgr;
}

void memoryMgr_cleanupAll(MemoryMgr* mgr) {
  assert(mgr != NULL);

  for (int i = 0; i < mgr->nPtrs; i++) {
    if (mgr->ptrs[i] != NULL) {
      free(mgr->ptrs[i]);
      mgr->ptrs[i] = NULL;
    }
  }
  if (mgr->ptrs != NULL) {
    free(mgr->ptrs);
  }
  free(mgr);
}

int memoryMgr_addPtr(MemoryMgr* mgr, void* ptr) {
  assert(mgr != NULL);

  void** newPtrs = (void**)realloc(mgr->ptrs, (mgr->nPtrs + 1) * sizeof(void*));
  if (newPtrs == NULL) {
    printf(ERROR_MEMALLOC);
    return -1;
  }

  mgr->ptrs = newPtrs;
  mgr->ptrs[mgr->nPtrs] = ptr;
  mgr->nPtrs++;
  return 0;
}

void* memoryMgr_malloc(MemoryMgr* mgr, size_t size) {
  void* ptr = malloc(size);
  if (ptr == NULL) {
    printf(ERROR_MEMALLOC);
    return NULL;
  }

  // effectively always a calloc for safety
  memset(ptr, 0, size);

  if (memoryMgr_addPtr(mgr, ptr)) {
    free(ptr);
    return NULL;
  }

  return ptr;
}

void* memoryMgr_realloc(MemoryMgr* mgr, void* oldPtr, size_t newSize) {
  void* newPtr = realloc(oldPtr, newSize);
  if (newPtr == NULL) {
    printf(ERROR_MEMALLOC);
    return NULL;
  }

  // Tiny optimisation here: if the oldPtr is NULL, then we can reuse an empty
  // spot if found
  for (int i = 0; i < mgr->nPtrs; i++) {
    if (mgr->ptrs[i] == oldPtr) {
      mgr->ptrs[i] = newPtr;
      return newPtr;
    }
  }

  // Not found, add new one
  if (memoryMgr_addPtr(mgr, newPtr)) {
    free(newPtr);
    return NULL;
  }
  return newPtr;
}

void* memoryMgr_free(MemoryMgr* mgr, void* ptr) {
  assert(mgr != NULL);

  for (int i = 0; i < mgr->nPtrs; i++) {
    if (mgr->ptrs[i] == ptr) {
      free(mgr->ptrs[i]);
      mgr->ptrs[i] = NULL;
    }
  }

  // TODO: compact the ptrs array

  return NULL;  // useful to set the pointer to NULL in the caller
}
