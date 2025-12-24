#ifndef OPTREE_H
#define OPTREE_H

#include "bmpRepo.h"
#include "memoryMgr.h"

typedef enum BlendMode {
  // Same values as the command letters for easier parsing
  BLEND_NORMAL = 'n',
  BLEND_MUL = 'm',
  BLEND_SUB = 's',
} BlendMode;

/// @brief Image operation tree node (N-ary tree) implemented with dynamic
/// arrays
typedef struct OpTreeNode {
  int layerId;  // Unique identifier for this layer

  // The actual operation
  int bmpId;                    // ID of the BMP being placed
  BmpRepoEntry* associatedBmp;  // Pointer to the BMPRepoEntry for easy access
  int destX,
      destY;  // Destination coordinates on the canvas (relative to top-left)
  BlendMode blendMode;  // Blending mode to use

  // Tree structure
  struct OpTreeNode* parent;     // NULL for root
  struct OpTreeNode** children;  // Dynamic array of child nodes
  int nChildren;
} OpTreeNode;

/// @brief Holds the tree root, some pointers and utilities
typedef struct OpTree {
  OpTreeNode* root;     // Root node
  OpTreeNode* current;  // Pointer to currently active node
  int lastLayerId;      // Last used layer ID, used to assign new ones

  // We keep the MemoryMgr here to make memory management streamlined
  MemoryMgr* mgr;
} OpTree;

/// @brief Initializes an empty OpTree
/// @param mgr An instance of MemoryMgr to use for allocations
/// @return A pointer to the created OpTree, or NULL if memory allocation failed
OpTree* opTree_init(MemoryMgr* mgr);

/// @brief Creates a new OpTreeNode (without adding it to the tree)
/// @param tree The OpTree this node will belong to
/// @param parent The parent node (NULL for root)
/// @param layerId The unique layer ID for this node
/// @return A pointer to the created OpTreeNode, or NULL if memory allocation
/// failed
OpTreeNode* opTree_createNode(OpTree* tree, OpTreeNode* parent, int layerId);

/// @brief Appends a child node to another node in the tree
/// @param tree The OpTree this node belongs to
/// @param parent The parent node
/// @param child The child node to append
/// @return 0 on success, -1 if memory allocation failed
int opTree_appendChildNode(OpTree* tree, OpTreeNode* parent, OpTreeNode* child);

/// @brief Creates and appends a new tree node as child of the currently active one
/// @param tree The OpTree this node belongs to
/// @param bmpId The ID of the BMP being placed
/// @param associatedBmp A pointer to the associated BmpRepoEntry
/// @param destX Destination X coordinate on the canvas
/// @param destY Destination Y coordinate on the canvas
/// @param blendMode The blending mode to use
/// @return A pointer to the created OpTreeNode, or NULL if memory allocation
/// failed
OpTreeNode* opTree_appendNewToCurrent(OpTree* tree, int bmpId,
                                      BmpRepoEntry* associatedBmp, int destX,
                                      int destY, BlendMode blendMode);

// Returns 0 on success, -1 if already at root
int opTree_undo(OpTree* tree);

OpTreeNode* opTree_findId(OpTreeNode* tree, int layerId);

// Returns 0 on success, -1 if not found
int opTree_switch(OpTree* tree, int layerId);

void opTree_printRecursive(OpTreeNode* node, int depth);

void opTree_renderRecursive(OpTreeNode* node, uint8_t* buffer, BmpRepo* bmpRepo,
                            int canvasWidth, int canvasHeight);

uint8_t* opTree_renderCurrent(OpTree* tree, BmpRepo* bmpRepo, int canvasWidth,
                              int canvasHeight);

#endif  // OPTREE_H
