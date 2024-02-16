#include "rendering.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "util.h"

void render_fps(SDL_Renderer *renderer, TTF_Font *font, int fps)
{
  SDL_Color color = {255, 255, 255, 255}; // White color for the FPS counter text
  char fpsText[15];
  sprintf(fpsText, "FPS: %d", fps); // Create a string for the FPS text

  SDL_Surface *surface = TTF_RenderText_Solid(font, fpsText, color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect textRect = {5, 5, surface->w, surface->h}; // Position: top left
  SDL_FreeSurface(surface);                           // No longer needed

  SDL_RenderCopy(renderer, texture, NULL, &textRect);
  SDL_DestroyTexture(texture); // Clean up
}


void draw(
  SDL_Renderer *renderer,
  SDL_Texture *texture,
  Uint32 *pixels,
  int screenWidth,
  int screenHeight
) {
  Uint8 r, g, b;
  unsigned int seed = SDL_GetTicks();

  for (int i = 0; i < screenWidth * screenHeight; i++)
  {
    // Uint8 r = rand() % 256;
    // Uint8 g = rand() % 256;
    // Uint8 b = rand() % 256;
    // unsigned int randVal = rand_r(&seed); // Use reentrant rand for potential thread-safety
    unsigned int r = rand_r(&seed);
    r = (r) & 0xFF;
    g = ((r >> 8) & 0xFF);
    b = ((r >> 16) & 0xFF);
    pixels[i] = (255 << 24) + (r << 16) + (g << 8) + b; // Assuming ARGB8888 format
  }
}
