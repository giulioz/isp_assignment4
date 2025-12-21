#include <SDL.h>
#include <stdio.h>

#include "bmpRepo.h"

void SDL_DisplayImage(BmpRepoEntry* entry) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL_Init Error: %s\n", SDL_GetError());
    return;
  }

  SDL_Window* win = SDL_CreateWindow("BMP Display", SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED, (int)entry->width,
                                     (int)entry->height, SDL_WINDOW_SHOWN);
  if (win == NULL) {
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
    SDL_Quit();
    return;
  }

  SDL_Renderer* ren = SDL_CreateRenderer(
      win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == NULL) {
    SDL_DestroyWindow(win);
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_Quit();
    return;
  }

  SDL_Surface* bmp = SDL_LoadBMP("images/city_highres.bmp");
  if (bmp == NULL) {
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
    SDL_Quit();
    return;
  }

  SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, bmp);
  SDL_FreeSurface(bmp);
  if (tex == NULL) {
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    SDL_Quit();
    return;
  }

  SDL_RenderClear(ren);
  SDL_RenderCopy(ren, tex, NULL, NULL);
  SDL_RenderPresent(ren);

  while (1) {
    SDL_Event e;
    if (SDL_WaitEvent(&e)) {
      if (e.type == SDL_QUIT) {
        break;
      }
    }
  }

  SDL_DestroyTexture(tex);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
}

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

  SDL_DisplayImage(entry);

  bmpRepo_free(repo);

  return 0;
}
