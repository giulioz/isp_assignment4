#ifndef OPTREE_H
#define OPTREE_H

#include "bmpRepo.h"
#include "cleanupMgr.h"

typedef enum BlendMode {
  BLEND_NORMAL,
  BLEND_MUL,
  BLEND_SUB,
} BlendMode;

typedef struct OpTreeNode {
  int layerId;

  BmpRepoEntry* associatedBmp;
  int destX, destY;
  int srcX, srcY;
  int clipWidth, clipHeight;
  BlendMode blendMode;

  // Tree structure
  struct OpTreeNode* parent;  // NULL for root
  struct OpTreeNode** children;
  int nChildren;
} OpTreeNode;

// Returns NULL if failed
OpTreeNode* opTree_createNode(OpTreeNode* parent, int layerId, CleanupMgr* mgr);

// Returns NULL if failed
OpTreeNode* opTree_createRoot(CleanupMgr* mgr);

// Returns -1 if failed
int opTree_appendChildNode(OpTreeNode* parent, OpTreeNode* child,
                           CleanupMgr* mgr);

BmpRepoEntry* opTree_renderBranch(OpTreeNode* endpoint, BmpRepo* bmpRepo,
                                  CleanupMgr* mgr);

#endif  // OPTREE_H
