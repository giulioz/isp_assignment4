#include <SDL.h>
#include <stdio.h>

#include "bmpRepo.h"
#include "sdlViewer.h"

int main(void) {
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
