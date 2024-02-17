#ifndef RENDERING_H
#define RENDERING_H

#include "SDL.h"
#include "SDL_ttf.h"

void render_fps(SDL_Renderer *renderer, TTF_Font *font, int fps);
void draw(SDL_Renderer *renderer, SDL_Texture *texture, Uint32 *pixels, int screenWidth, int screenHeight);
void draw_text(SDL_Renderer *renderer, TTF_Font *font, int screenWidth, int screenHeight);
void render_stat(SDL_Renderer *renderer, TTF_Font *font, char *s, int x, int y);

#endif
