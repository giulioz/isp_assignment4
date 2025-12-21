#include "opTree.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

OpTreeNode* opTree_createNode(OpTreeNode* parent, int layerId) {
  OpTreeNode* node = (OpTreeNode*)malloc(sizeof(OpTreeNode));
  if (node != NULL) {
    node->parent = parent;
    node->children = NULL;
    node->nChildren = 0;
    node->layerId = layerId;
    node->associatedBmp = NULL;
  } else {
    printf("[ERROR] Memory allocation failed!\n\n");
  }
  return node;
}

OpTreeNode* opTree_createRoot(void) { return opTree_createNode(NULL, 0); }

int opTree_appendChildNode(OpTreeNode* parent, OpTreeNode* child) {
  assert(parent != NULL);
  assert(child != NULL);

  OpTreeNode** newChildren = (OpTreeNode**)realloc(
      parent->children, (parent->nChildren + 1) * sizeof(OpTreeNode*));
  if (newChildren == NULL) {
    printf("[ERROR] Memory allocation failed!\n\n");
    return -1;
  }

  parent->children = newChildren;
  parent->children[parent->nChildren] = child;

  child->parent = parent;

  parent->nChildren++;

  return 0;
}

void opTree_freeNode(OpTreeNode* node) {
  if (node != NULL) {
    if (node->children != NULL) {
      free(node->children);
    }
    free(node);
  }
}

void opTree_freeTree(OpTreeNode* root) {
  if (root != NULL) {
    if (root->children != NULL) {
      for (size_t i = 0; root->children[i] != NULL; ++i) {
        opTree_freeTree(root->children[i]);
      }
      free(root->children);
    }
    free(root);
  }
}

BmpRepoEntry* opTree_renderBranch(OpTreeNode* endpoint, BmpRepo* bmpRepo) {}
