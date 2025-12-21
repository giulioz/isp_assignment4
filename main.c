#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmpRepo.h"
#include "cleanupMgr.h"
#include "opTree.h"
#include "sdlViewer.h"

int main(int argc, char* argv[]) {
  int canvasWidth = 0, canvasHeight = 0;
  BmpRepo* bmpRepo;
  OpTreeNode* rootOpNode;
  CleanupMgr* cleanupMgr;

  cleanupMgr = cleanupMgr_init();
  if (cleanupMgr == NULL) {
    return 1;
  }

  if (argc != 3) {
    printf("[ERROR] Invalid amount of command line parameters!\n");
    return 2;
  }

  if (sscanf(argv[1], "%i", &canvasWidth) != 1) {
    printf("[ERROR] Invalid canvas size specified!\n");
    return 3;
  }
  if (sscanf(argv[2], "%i", &canvasHeight) != 1) {
    printf("[ERROR] Invalid canvas size specified!\n");
    return 3;
  }

  printf(
      "\nWelcome to Image Structuring Program!\nThe canvas is %d x "
      "%d pixels.\n\n",
      canvasWidth, canvasHeight);

  bmpRepo = bmpRepo_init(cleanupMgr);
  if (bmpRepo == NULL) {
    cleanupMgr_cleanupAll(cleanupMgr);
    return 1;
  }

  rootOpNode = opTree_createRoot(cleanupMgr);
  if (rootOpNode == NULL) {
    cleanupMgr_cleanupAll(cleanupMgr);
    return 1;
  }

  // BmpRepoEntry* entry = bmpRepo_loadFromFile(repo,
  // "images/city_highres.bmp"); if (entry == NULL) {
  //   printf("error: failed to load BMP from file\n");
  //   bmpRepo_free(repo);
  //   return 1;
  // }

  // bmpEntryDisplayImage(entry);

  cleanupMgr_cleanupAll(cleanupMgr);

  return 0;
}
