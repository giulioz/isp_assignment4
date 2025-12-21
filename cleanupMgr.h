#ifndef CLEANUPMGR_H
#define CLEANUPMGR_H

typedef struct CleanupMgr {
  void** ptrs;
  int nPtrs;
} CleanupMgr;

CleanupMgr* cleanupMgr_init(void);
void cleanupMgr_cleanupAll(CleanupMgr* mgr);
void cleanupMgr_addPtr(CleanupMgr* mgr, void* ptr);
void cleanupMgr_replacePtr(CleanupMgr* mgr, void* oldPtr, void* newPtr);
void* cleanupMgr_freeSingle(CleanupMgr* mgr, void* ptr);

#endif  // CLEANUPMGR_H
