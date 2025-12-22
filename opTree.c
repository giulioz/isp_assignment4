#include "opTree.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "strings.h"

OpTree* opTree_init(MemoryMgr* mgr) {
  assert(mgr != NULL);

  OpTree* tree = (OpTree*)memoryMgr_malloc(mgr, sizeof(OpTree));
  if (tree != NULL) {
    tree->mgr = mgr;

    tree->root = opTree_createNode(tree, NULL, 0);
    if (tree->root == NULL) {
      return NULL;
    }
    tree->current = tree->root;
    tree->lastLayerId = 0;
  } else {
    printf(ERROR_MEMALLOC);
  }
  return tree;
}

OpTreeNode* opTree_createNode(OpTree* tree, OpTreeNode* parent, int layerId) {
  assert(tree != NULL);

  OpTreeNode* node =
      (OpTreeNode*)memoryMgr_malloc(tree->mgr, sizeof(OpTreeNode));
  if (node != NULL) {
    node->parent = parent;
    node->children = NULL;
    node->nChildren = 0;
    node->layerId = layerId;
    node->bmpId = -1;
    node->associatedBmp = NULL;
    node->destX = 0;
    node->destY = 0;
    node->blendMode = BLEND_NORMAL;
  } else {
    printf(ERROR_MEMALLOC);
  }
  return node;
}

int opTree_appendChildNode(OpTree* tree, OpTreeNode* parent,
                           OpTreeNode* child) {
  assert(tree != NULL);
  assert(parent != NULL);
  assert(child != NULL);

  OpTreeNode** newChildren = (OpTreeNode**)memoryMgr_realloc(
      tree->mgr, parent->children,
      (parent->nChildren + 1) * sizeof(OpTreeNode*));
  if (newChildren == NULL) {
    printf(ERROR_MEMALLOC);
    return -1;
  }

  parent->children = newChildren;
  parent->children[parent->nChildren] = child;

  child->parent = parent;

  parent->nChildren++;

  return 0;
}

OpTreeNode* opTree_appendNewToCurrent(OpTree* tree, int bmpId,
                                      BmpRepoEntry* associatedBmp, int destX,
                                      int destY, BlendMode blendMode) {
  assert(tree != NULL);

  // Create and fill new node
  OpTreeNode* child =
      opTree_createNode(tree, tree->current, tree->lastLayerId + 1);
  if (child == NULL) {
    return NULL;
  }
  child->bmpId = bmpId;
  child->associatedBmp = associatedBmp;
  child->destX = destX;
  child->destY = destY;
  child->blendMode = blendMode;
  tree->lastLayerId++;

  int result = opTree_appendChildNode(tree, tree->current, child);
  if (result != 0) {
    return NULL;
  }

  tree->current = child;
  return child;
}

BmpRepoEntry* opTree_renderBranch(OpTree* tree, OpTreeNode* endpoint,
                                  BmpRepo* bmpRepo) {}
