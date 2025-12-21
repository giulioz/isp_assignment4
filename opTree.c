#include "opTree.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "strings.h"

OpTreeNode* opTree_createNode(OpTreeNode* parent, int layerId,
                              CleanupMgr* mgr) {
  assert(mgr != NULL);

  OpTreeNode* node = (OpTreeNode*)malloc(sizeof(OpTreeNode));
  if (node != NULL) {
    node->parent = parent;
    node->children = NULL;
    node->nChildren = 0;
    node->layerId = layerId;
    node->associatedBmp = NULL;
    node->destX = 0;
    node->destY = 0;
    node->srcX = 0;
    node->srcY = 0;
    node->clipWidth = 0;
    node->clipHeight = 0;
    node->blendMode = BLEND_NORMAL;
    cleanupMgr_addPtr(mgr, node);
  } else {
    printf(ERROR_MEMALLOC);
  }
  return node;
}

OpTreeNode* opTree_createRoot(CleanupMgr* mgr) {
  return opTree_createNode(NULL, 0, mgr);
}

int opTree_appendChildNode(OpTreeNode* parent, OpTreeNode* child,
                           CleanupMgr* mgr) {
  assert(parent != NULL);
  assert(child != NULL);

  OpTreeNode** newChildren = (OpTreeNode**)realloc(
      parent->children, (parent->nChildren + 1) * sizeof(OpTreeNode*));
  if (newChildren == NULL) {
    printf(ERROR_MEMALLOC);
    return -1;
  }
  cleanupMgr_replacePtr(mgr, parent->children, newChildren);

  parent->children = newChildren;
  parent->children[parent->nChildren] = child;

  child->parent = parent;

  parent->nChildren++;

  return 0;
}

BmpRepoEntry* opTree_renderBranch(OpTreeNode* endpoint, BmpRepo* bmpRepo,
                                  CleanupMgr* mgr) {}
