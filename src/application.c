#include <stdio.h>
#include <SDL.h>

void init_sdl() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
  }
  else
  {
    SDL_Window *window = SDL_CreateWindow("Hello SDL World", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
      // Wait 5 seconds
      SDL_Delay(5000);
    }
    SDL_DestroyWindow(window);
  }
  SDL_Quit();
}
