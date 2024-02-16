#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h> // Include SDL_ttf for font rendering
#include "application.h"
#include "event_loop.h"

int w_width = 1280;
int w_height = 800;

void init_sdl()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() < 0)
  {
    printf("SDL or SDL_ttf could not initialize! SDL_Error: %s\n", SDL_GetError());
    return;
  }


  SDL_Window *window = SDL_CreateWindow("Mandelbrocc", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w_width, w_height, SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (window == NULL || renderer == NULL)
  {
    printf("Window or Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
  }
  else
  {
    TTF_Font *font = TTF_OpenFont("./assets/mononoki-Regular.ttf", 24); // Load your font
    if (!font)
    {
      printf("Failed to load font: %s\n", TTF_GetError());
    }
    else
    {
      event_loop(window, renderer, font);
      TTF_CloseFont(font); // Clean up font
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
  }
  TTF_Quit();
  SDL_Quit();
}
