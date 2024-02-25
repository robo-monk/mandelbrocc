#ifndef RENDERING_H
#define RENDERING_H

#include "SDL.h"
#include "SDL_ttf.h"

void rendering_setup(int screenWidth, int screenHeight,
                     SDL_Renderer *sdl_renderer, TTF_Font *ttf_font);
void render_fps(SDL_Renderer *renderer, TTF_Font *font, int fps);
void draw(Uint32 *pixels, int screenWidth, int screenHeight);
void draw_text(SDL_Renderer *renderer, TTF_Font *font, int screenWidth,
               int screenHeight);
void render_stat(SDL_Renderer *renderer, TTF_Font *font, char *s, int x, int y);
void handle_keyboard_event(SDL_KeyboardEvent *event);

#endif
