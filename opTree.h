#ifndef OPTREE_H
#define OPTREE_H

#include "bmpRepo.h"

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
  struct OpTreeNode *parent; // NULL for root
  struct OpTreeNode **children;
  int nChildren;
} OpTreeNode;

// Returns NULL if failed
OpTreeNode *opTree_createNode(OpTreeNode *parent, int layerId);

// Returns NULL if failed
OpTreeNode *opTree_createRoot(void);

// Returns -1 if failed
int opTree_appendChildNode(OpTreeNode *parent, OpTreeNode *child);

// Will NOT free the bitmaps
void opTree_freeNode(OpTreeNode *node);
void opTree_freeTree(OpTreeNode *root);

BmpRepoEntry* opTree_renderBranch(OpTreeNode *endpoint, BmpRepo* bmpRepo);

#endif // OPTREE_H
