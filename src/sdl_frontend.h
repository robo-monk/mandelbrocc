#ifndef SDL_FRONT_H
#define SDL_FRONT_H

#include "rendering.h"
#include <SDL.h>
#include <SDL_ttf.h>

void __sdl_setup(SDL_Renderer *sdl_renderer, TTF_Font *ttf_font);
void render_stat(char *s, int x, int y);
void handle_keydown_event(SDL_KeyboardEvent *event);
void handle_keyup_event(SDL_KeyboardEvent *event);

// #include "SDL.h"

// void render_data(
//     double *data,
//     int data_rows, int data_cols,

//     Uint32 *pixels,
//     int image_width, int image_height
// );

#endif
