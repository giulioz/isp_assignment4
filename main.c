#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmpRepo.h"
#include "sdlViewer.h"

int main(int argc, char* argv[]) {
  int canvasWidth = 0, canvasHeight = 0;

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

  BmpRepo* repo = bmpRepo_init();
  if (repo == NULL) {
    printf("error: failed to initialize BmpRepo\n");
    return 1;
  }

  BmpRepoEntry* entry = bmpRepo_loadFromFile(repo, "images/city_highres.bmp");
  if (entry == NULL) {
    printf("error: failed to load BMP from file\n");
    bmpRepo_free(repo);
    return 1;
  }

  bmpEntryDisplayImage(entry);

  bmpRepo_free(repo);

  return 0;
}
