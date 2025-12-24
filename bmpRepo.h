#ifndef BMP_REPO_H
#define BMP_REPO_H

#include <stdint.h>
#include <stdlib.h>

#include "bmp.h"
#include "memoryMgr.h"

// Implements a simple BMP repository to hold loaded BMPs in memory
// and provide utilities to create empty BMPs, retrieve them by ID, etc.
// It automatically manages memory using the provided MemoryMgr instance.

// 4 bytes per pixel (ARGB)
#define BMP_STRIDE 4

/// @brief An entry in the BMP repository, representing a loaded BMP image
typedef struct BmpRepoEntry {
  // Unique identifier for this BMP
  int bmpId;

  // Pointer to pixel data (size: width * height * BMP_STRIDE)
  uint8_t* pixelData;

  // Image size
  int width;
  int height;

  // TODO: support cropping without copying data
} BmpRepoEntry;

/// @brief Instance of the BMP repository, keeping the bitmap data pointers and
/// utilities
typedef struct BmpRepo {
  // Last used BMP ID, used to assign new ones
  int lastBmpId;

  // We use a dynamic array of pointers to make reallocations easier
  // We used a dynamic array of structs before, but that would cause problems if
  // createEmptyBmp is called after getBmpById, as the pointer returned by
  // getBmpById could be invalidated by a realloc in createEmptyBmp.
  BmpRepoEntry** entries;

  // We keep the MemoryMgr here to make memory management streamlined
  MemoryMgr* mgr;
} BmpRepo;

/// @brief Initializes the BMP repository
/// @param mgr An instance of MemoryMgr to use for allocations
/// @return NULL if some memory allocation failed, otherwise a pointer to the created BMP repository
/// initialized BmpRepo
BmpRepo* bmpRepo_init(MemoryMgr* mgr);

/// @brief Allocates and adds a new empty BMP with the specified dimensions
/// @param repo The destination BMP repository
/// @param width Image width
/// @param height Image height
/// @return A pointer to the created BmpRepoEntry, or NULL if memory allocation
/// failed
BmpRepoEntry* bmpRepo_createEmptyBmp(BmpRepo* repo, int width, int height);

/// @brief Retrieves a BMP entry by its unique identifier
/// @param repo The BMP repository to search in
/// @param bmpId The specific BMP ID to search for
/// @return A pointer to the BmpRepoEntry with the specified ID, or NULL if not
/// found
BmpRepoEntry* bmpRepo_getBmpById(BmpRepo* repo, int bmpId);

#endif  // BMP_REPO_H
