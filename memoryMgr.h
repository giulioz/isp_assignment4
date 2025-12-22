#ifndef MEMORYMGR_H
#define MEMORYMGR_H

#include <stdlib.h>

// Simple memory manager to track allocated pointers and free them all at once
// It works by keeping a dynamic array with all the pointers
// Its API is similar to malloc/realloc/free, but keeps track of what needs to
// be freed It's also possible to free memory manually, leaving empty spots in
// the pointers array, which can be reused

typedef struct MemoryMgr {
  // Essentially a dynamic array of allocations
  void** ptrs;
  int nPtrs;
} MemoryMgr;

/// @brief Initializes the memory manager
/// @return a pointer to memory manager instance, or NULL on failure
MemoryMgr* memoryMgr_init(void);

/// @brief Fres all allocated memory tracked by the memory manager, including the memory manager itself
/// @param mgr The current memory manager instance
void memoryMgr_cleanupAll(MemoryMgr* mgr);

/// @brief Works like malloc (actually calloc), but tracked by the memory manager and with error handling
/// @param mgr The current memory manager instance
/// @param newSize The new size in bytes
/// @return a pointer to the allocated memory, or NULL on failure
void* memoryMgr_malloc(MemoryMgr* mgr, size_t size);

/// @brief Works like realloc, but tracked by the memory manager and with error handling
/// @param mgr The current memory manager instance
/// @param oldPtr The pointer to realloc
/// @param newSize The new size in bytes
/// @return a pointer to the reallocated memory, or NULL on failure
void* memoryMgr_realloc(MemoryMgr* mgr, void* oldPtr, size_t newSize);

/// @brief Forces the memory manager to free a specific pointer
/// @param mgr The current memory manager instance
/// @param ptr The pointer to free
/// @return always NULL, useful to set the pointer to NULL in the caller
void* memoryMgr_free(MemoryMgr* mgr, void* ptr);

/// @brief Manually adds a pointer to be tracked by the memory manager, if it has been allocated elsewhere
/// @param mgr The current memory manager instance
/// @param ptr The pointer to add
/// @return 0 on success, -1 on failure
int memoryMgr_addPtr(MemoryMgr* mgr, void* ptr);

#endif  // MEMORYMGR_H
