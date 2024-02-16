#include "event_loop.h"
#include "rendering.h"
#include "util.h"

void event_loop(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font)
{
  Uint32 startTick, endTick = 0;
  Uint32 secondStart = SDL_GetTicks(), fps = 0;

  SDL_bool done = SDL_FALSE;
  while (!done)
  {
    startTick = SDL_GetTicks();

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        done = SDL_TRUE;
      }
    }

    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    int w_width, w_height;
    SDL_GetWindowSize(window, &w_width, &w_height);

    // Your drawing code here (or call a function)
    draw(renderer, w_width, w_height);


    endTick = SDL_GetTicks();
    fps = 1000/(endTick - startTick);
    render_fps(renderer, font, fps);
    SDL_RenderPresent(renderer);
  }
}
