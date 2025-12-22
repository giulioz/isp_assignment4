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

// Returns 0 on success, -1 if already at root
int opTree_undo(OpTree* tree);

// Returns 0 on success, -1 if not found
int opTree_switch(OpTree* tree, int layerId);

void opTree_printRecursive(OpTreeNode* node, int depth);

void opTree_renderRecursive(OpTreeNode* node, uint8_t* buffer, BmpRepo* bmpRepo,
                            int canvasWidth, int canvasHeight);

uint8_t* opTree_renderCurrent(OpTree* tree, BmpRepo* bmpRepo, int canvasWidth,
                              int canvasHeight);

#endif  // OPTREE_H
