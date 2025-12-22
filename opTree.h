#ifndef OPTREE_H
#define OPTREE_H

#include "bmpRepo.h"
#include "memoryMgr.h"

typedef enum BlendMode {
  BLEND_NORMAL = 'n',
  BLEND_MUL = 'm',
  BLEND_SUB = 's',
} BlendMode;

typedef struct OpTreeNode {
  int layerId;

  int bmpId;
  BmpRepoEntry* associatedBmp;
  int destX, destY;
  BlendMode blendMode;

  // Tree structure
  struct OpTreeNode* parent;  // NULL for root
  struct OpTreeNode** children;
  int nChildren;
} OpTreeNode;

typedef struct OpTree {
  OpTreeNode* root;
  OpTreeNode* current;
  int lastLayerId;

  // We keep the MemoryMgr here to make memory management streamlined
  MemoryMgr* mgr;
} OpTree;

// Returns NULL if failed
OpTree* opTree_init(MemoryMgr* mgr);

// Returns NULL if failed
OpTreeNode* opTree_createNode(OpTree* tree, OpTreeNode* parent, int layerId);

// Returns -1 if failed
int opTree_appendChildNode(OpTree* tree, OpTreeNode* parent, OpTreeNode* child);

// Returns NULL if failed
OpTreeNode* opTree_appendNewToCurrent(OpTree* tree, int bmpId,
                                      BmpRepoEntry* associatedBmp, int destX,
                                      int destY, BlendMode blendMode);

BmpRepoEntry* opTree_renderBranch(OpTree* tree, OpTreeNode* endpoint,
                                  BmpRepo* bmpRepo);

#endif  // OPTREE_H
