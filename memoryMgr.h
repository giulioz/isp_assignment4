#ifndef MEMORYMGR_H
#define MEMORYMGR_H

#include <stdlib.h>

typedef struct MemoryMgr {
  void** ptrs;
  int nPtrs;
} MemoryMgr;

MemoryMgr* memoryMgr_init(void);
void memoryMgr_cleanupAll(MemoryMgr* mgr);
void* memoryMgr_malloc(MemoryMgr* mgr, size_t size);
void* memoryMgr_realloc(MemoryMgr* mgr, void* oldPtr, size_t newSize);
void* memoryMgr_free(MemoryMgr* mgr, void* ptr);
int memoryMgr_addPtr(MemoryMgr* mgr, void* ptr);

#endif  // MEMORYMGR_H
