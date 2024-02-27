#include "sdl_frontend.h"
#include "frontend.h"
#include "rendering.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdarg.h>

SDL_Renderer *renderer;
TTF_Font *font;

void __sdl_setup(SDL_Renderer *sdl_renderer, TTF_Font *ttf_font) {
  renderer = sdl_renderer;
  font = ttf_font;
}

void render_stat(char *s, int x, int y) {
  // return;
  SDL_Color c = {255, 255, 255, 255};

  SDL_Surface *surface = TTF_RenderText_Solid(font, s, c);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect textRect = {x, y, surface->w, surface->h};
  SDL_FreeSurface(surface); // No longer needed

  SDL_RenderCopy(renderer, texture, NULL, &textRect);
  SDL_DestroyTexture(texture); // Clean up
}

void __platform_render_text_ui(TextUI *text_ui, ...) {
  char str[256];
  va_list args;
  va_start(args, text_ui->format_str);
  vsnprintf(str, sizeof(str), text_ui->format_str, args);
  va_end(args);

  render_stat(str, text_ui->x, text_ui->y);
};

void handle_keydown_event(SDL_KeyboardEvent *key) {
  Action action = NOP;
  switch (key->keysym.sym) {
  case SDLK_LEFT:
    action = LEFT;
    break;
  case SDLK_RIGHT:
    action = RIGHT;
    break;
  case SDLK_DOWN:
    action = DOWN;
    break;
  case SDLK_UP:
    action = UP;
    break;
  case SDLK_EQUALS:
  case SDLK_PLUS:
    action = ZOOM_IN;
    break;
  case SDLK_MINUS:
    action = ZOOM_OUT;
    break;
  case SDLK_COMMA:
    action = MAX_ITERATIONS_DECR;
    break;
  case SDLK_PERIOD:
    action = MAX_ITERATIONS_INCR;
    break;
  default:
    break;
  }

  perform_action(action);
}

void handle_keyup_event(SDL_KeyboardEvent *key) {

  Action action = NOP;

  switch (key->keysym.sym) {
  case SDLK_h:
    action = HIDE_UI;
    break;
  default:
    break;
  }

  perform_action(action);
}
