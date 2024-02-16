#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include "SDL.h"
#include "SDL_ttf.h"

void event_loop(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font);
// void init_sdl();

#endif
