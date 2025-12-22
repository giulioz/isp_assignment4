#include "opTree.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strings.h"

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

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

int opTree_undo(OpTree* tree) {
  assert(tree != NULL);
  if (tree->current->parent != NULL) {
    tree->current = tree->current->parent;
    return 0;
  } else {
    return -1;
  }
}

OpTreeNode* opTree_findId(OpTreeNode* tree, int layerId) {
  if (tree->layerId == layerId) {
    return tree;
  }

  for (int i = 0; i < tree->nChildren; i++) {
    OpTreeNode* result = opTree_findId(tree->children[i], layerId);
    if (result != NULL) {
      return result;
    }
  }

  return NULL;
}

int opTree_switch(OpTree* tree, int layerId) {
  assert(tree != NULL);

  OpTreeNode* node = opTree_findId(tree->root, layerId);
  if (node != NULL) {
    tree->current = node;
    return 0;
  } else {
    return -1;
  }
}

void opTree_printRecursive(OpTreeNode* node, int depth) {
  for (int i = 0; i < depth; i++) {
    printf("   ");
  }

  if (node->parent == NULL) {
    // root node
    printf("Layer %d\n", node->layerId);
  } else {
    printf("Layer %d renders BMP %d at %d %d\n", node->layerId, node->bmpId,
           node->destX, node->destY);
  }

  for (int i = 0; i < node->nChildren; i++) {
    opTree_printRecursive(node->children[i], depth + 1);
  }
}

void opTree_renderRecursive(OpTreeNode* node, uint8_t* buffer, BmpRepo* bmpRepo,
                            int canvasWidth, int canvasHeight) {
  if (node->parent == NULL) {
    // Root node: fill with white
    memset(buffer, 255, BMP_STRIDE * canvasWidth * canvasHeight);
    return;
  }

  // Render parent first
  opTree_renderRecursive(node->parent, buffer, bmpRepo, canvasWidth,
                         canvasHeight);

  // Render
  for (int y = 0; y < node->associatedBmp->height; y++) {
    for (int x = 0; x < node->associatedBmp->width; x++) {
      int canvasX = node->destX + x;
      int canvasY = node->destY + y;

      if (canvasX < 0 || canvasY < 0 || canvasX >= canvasWidth ||
          canvasY >= canvasHeight) {
        // should never happen due to checks but let's test anyway
        continue;
      }

      int canvasIdx = (canvasY * canvasWidth + canvasX) * BMP_STRIDE;
      int bmpIdx = (y * node->associatedBmp->width + x) * BMP_STRIDE;

      uint8_t* destPixel = &buffer[canvasIdx];
      uint8_t* srcPixel = &node->associatedBmp->pixelData[bmpIdx];

      double srcAlpha = (double)srcPixel[3] / 255.0;
      double srcAlphaInv = 1.0 - srcAlpha;

      switch (node->blendMode) {
        case BLEND_NORMAL:
          destPixel[0] = (uint8_t)((double)srcPixel[0] * srcAlpha +
                                   (double)destPixel[0] * srcAlphaInv);
          destPixel[1] = (uint8_t)((double)srcPixel[1] * srcAlpha +
                                   (double)destPixel[1] * srcAlphaInv);
          destPixel[2] = (uint8_t)((double)srcPixel[2] * srcAlpha +
                                   (double)destPixel[2] * srcAlphaInv);
          break;
        case BLEND_MUL:
          destPixel[0] =
              (uint8_t)(((double)destPixel[0] * (double)srcPixel[0]) / 255.0);
          destPixel[1] =
              (uint8_t)(((double)destPixel[1] * (double)srcPixel[1]) / 255.0);
          destPixel[2] =
              (uint8_t)(((double)destPixel[2] * (double)srcPixel[2]) / 255.0);
          break;
        case BLEND_SUB:
          destPixel[0] =
              max(destPixel[0], srcPixel[0]) - min(destPixel[0], srcPixel[0]);
          destPixel[1] =
              max(destPixel[1], srcPixel[1]) - min(destPixel[1], srcPixel[1]);
          destPixel[2] =
              max(destPixel[2], srcPixel[2]) - min(destPixel[2], srcPixel[2]);
          break;
      }
    }
  }
}

uint8_t* opTree_renderCurrent(OpTree* tree, BmpRepo* bmpRepo, int canvasWidth,
                              int canvasHeight) {
  assert(tree != NULL);
  assert(bmpRepo != NULL);

  uint8_t* buffer = (uint8_t*)memoryMgr_malloc(
      tree->mgr, BMP_STRIDE * canvasWidth * canvasHeight);
  if (buffer == NULL) {
    return NULL;
  }

  opTree_renderRecursive(tree->current, buffer, bmpRepo, canvasWidth,
                         canvasHeight);

  return buffer;
}
