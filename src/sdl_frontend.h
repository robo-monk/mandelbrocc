#ifndef SDL_FRONT_H
#define SDL_FRONT_H

#include "rendering.h"
#include <SDL.h>
#include <SDL_ttf.h>

void render_text_ui(TextUI *text_ui, ...);
void render_stat(SDL_Renderer *renderer, TTF_Font *font, char *s, int x, int y);
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
