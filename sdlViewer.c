#include "sdlViewer.h"

#include <SDL.h>

void bmpEntryDisplayImage(BmpRepoEntry* entry) {
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

  SDL_Texture* tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_BGRA8888,
                                       SDL_TEXTUREACCESS_STREAMING,
                                       (int)entry->width, (int)entry->height);
  if (tex == NULL) {
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    printf("SDL_CreateTexture Error: %s\n", SDL_GetError());
    SDL_Quit();
    return;
  }

  uint8_t* pixels_tmp = malloc(entry->width * entry->height * BMP_STRIDE);
  for (size_t y = 0; y < entry->height; y++) {
    for (size_t x = 0; x < entry->width; x++) {
      size_t index = (y * entry->width + x) * BMP_STRIDE;
      size_t indexB = ((entry->height - 1 - y) * entry->width + x) * BMP_STRIDE;
      pixels_tmp[index + 0] = entry->pixelData[indexB + 3];  // R
      pixels_tmp[index + 1] = entry->pixelData[indexB + 2];  // G
      pixels_tmp[index + 2] = entry->pixelData[indexB + 1];  // B
      pixels_tmp[index + 3] = entry->pixelData[indexB + 0];  // A
    }
  }
  SDL_UpdateTexture(tex, NULL, pixels_tmp, (int)(entry->width * BMP_STRIDE));
  free(pixels_tmp);

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
