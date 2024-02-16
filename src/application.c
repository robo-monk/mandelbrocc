#include <stdio.h>
#include <SDL.h>

void event_loop(SDL_Surface *surface) {
  for (int y = 0; y < surface->h; y++) {
    for (int x = 0; x < surface->w; x++) {
      Uint32 color = SDL_MapRGB(surface->format, x % 255, y % 255, (x + y) % 255);
      int index = (y * surface->w) + x;

      Uint32 *pixels = (Uint32 *)surface->pixels;
      pixels[index] = color;
      // (surface->pixels)[index] = color;
    }
  }
}


void init_sdl() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
  }
  else
  {
    int w_width = 640;
    int w_height = 480;
    SDL_Window *window = SDL_CreateWindow("Mandelbrocc", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w_width, w_height, SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    } else {
      SDL_bool done = SDL_FALSE;
      while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
          if (event.type == SDL_QUIT) {
            done = SDL_TRUE;
          }
        }
        SDL_Surface* windowSurface = SDL_GetWindowSurface(window);
        event_loop(windowSurface);
        SDL_UpdateWindowSurface(window);
      }
      SDL_DestroyWindow(window);
    }
  }
  SDL_Quit();
}
