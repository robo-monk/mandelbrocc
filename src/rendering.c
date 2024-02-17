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

void render_stat(SDL_Renderer *renderer, TTF_Font *font, char* s, int x, int y)
{
  SDL_Color color = {255, 255, 255, 255}; // White color for the FPS counter text
  // char fpsText[15];
  // sprintf(s, "FPS: %d", fps); // Create a string for the FPS text

  SDL_Surface *surface = TTF_RenderText_Solid(font, s, color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect textRect = {x, y, surface->w, surface->h};
  SDL_FreeSurface(surface);                           // No longer needed

  SDL_RenderCopy(renderer, texture, NULL, &textRect);
  SDL_DestroyTexture(texture); // Clean up
}

mandelbrot_params m_params = { .focal_x = 0.0, .focal_y = 0.0, .zoom = 1.0 };
mandelbrot_params rendered_m_params;

void draw(
  SDL_Renderer *renderer,
  SDL_Texture *texture,
  Uint32 *pixels,
  int screenWidth,
  int screenHeight
) {
  if (!mandelbrot_params_eq(&m_params, &rendered_m_params)) {
    rendered_m_params = m_params;
    mandelbrot_render(pixels, screenWidth, screenHeight, &m_params);
  }
}

void draw_text(
  SDL_Renderer *renderer,
  TTF_Font *font,
  int screenWidth,
  int screenHeight
) {
  char focal_stat[64];
  sprintf(focal_stat, "%f, %fi", m_params.focal_x, m_params.focal_y);
  render_stat(renderer, font, focal_stat, 5, 25);
}
