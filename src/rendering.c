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
int MAX_RESOLUTION = 100;
int INITIAL_RESOLUTION = 25;
int current_resolution = 10;

void draw(
  SDL_Renderer *renderer,
  SDL_Texture *texture,
  Uint32 *pixels,
  int screenWidth,
  int screenHeight
) {
  if (!mandelbrot_params_eq(&m_params, &rendered_m_params)) {
    current_resolution = INITIAL_RESOLUTION;
    rendered_m_params = m_params;
    mandelbrot_render(pixels, screenWidth, screenHeight, &m_params, current_resolution);
  } else if (current_resolution < MAX_RESOLUTION) {
    current_resolution *= 2;
    mandelbrot_render(pixels, screenWidth, screenHeight, &m_params, current_resolution);
  }
}

void draw_text(
  SDL_Renderer *renderer,
  TTF_Font *font,
  int screenWidth,
  int screenHeight
) {
  char focal_stat[64];
  sprintf(focal_stat, "%f, %fi @%f", m_params.focal_x, m_params.focal_y,m_params.zoom);
  render_stat(renderer, font, focal_stat, 5, 25);
}

void handle_keyboard_event(
  SDL_KeyboardEvent *key
) {
  double MOVE_SENSITIVITY = .1;
  double move_nip = MOVE_SENSITIVITY/m_params.zoom;
  switch (key->keysym.sym) {
    case SDLK_LEFT:
      m_params.focal_x -= move_nip; 
      break;
    case SDLK_RIGHT:
      m_params.focal_x += move_nip;
      break;
    case SDLK_DOWN:
      m_params.focal_y -= move_nip;
      break;
    case SDLK_UP:
      m_params.focal_y += move_nip; 
      break;
    case SDLK_EQUALS:
    case SDLK_PLUS:
      m_params.zoom *= 1.1;
      // m_params.zoom *= 1.1;
      break;
    case SDLK_MINUS:
      m_params.zoom /= 1.1;
      break;
    default:break;
  }
}
