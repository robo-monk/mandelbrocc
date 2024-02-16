#include "rendering.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "util.h"
#include "mandelbrot.h"

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
  mandelbrot_render(pixels, screenWidth, screenHeight, 0.0, 0.0, 1.0);
  // Uint8 r, g, b;
  // unsigned int seed = SDL_GetTicks();

  // for (int i = 0; i < screenWidth * screenHeight; i++)
  // {
  //   unsigned int r = rand_r(&seed);
  //   unsigned int g = rand_r(&seed);
  //   unsigned int b = rand_r(&seed);
  //   r = (r) & 0xFF;
  //   g = (g) & 0xFF;
  //   b = (b) & 0xFF;
  //   pixels[i] = (255 << 24) + (r << 16) + (g << 8) + b; // Assuming ARGB8888 format
  // }
}
