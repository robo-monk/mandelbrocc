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




void draw(SDL_Renderer *renderer, int screenWidth, int screenHeight)
{
  // Create a texture for pixel manipulation
  SDL_Texture *texture = SDL_CreateTexture(renderer,
                                           SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           screenWidth, screenHeight);

  // Allocate a buffer for pixel data
  Uint32 *pixels = malloc(screenWidth * screenHeight * sizeof(Uint32));
  if (!pixels)
  {
    // Handle allocation failure
    return;
  }

  // Fill the buffer with random colors
  for (int i = 0; i < screenWidth * screenHeight; i++)
  {
    Uint8 r = rand() % 256;
    Uint8 g = rand() % 256;
    Uint8 b = rand() % 256;
    pixels[i] = (255 << 24) + (r << 16) + (g << 8) + b; // Assuming ARGB8888 format
  }

  // Update texture with pixel buffer
  SDL_UpdateTexture(texture, NULL, pixels, screenWidth * sizeof(Uint32));

  // Free the pixel buffer
  free(pixels);

  // Copy the texture to the renderer
  SDL_RenderCopy(renderer, texture, NULL, NULL);

  // Destroy the texture after use
  SDL_DestroyTexture(texture);
}
