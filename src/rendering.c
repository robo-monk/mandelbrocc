#include "rendering.h"
#include "frontend.h"
#include "mandelbrot.h"
#include "util.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>

// void render_fps(SDL_Renderer *renderer, TTF_Font *font, int fps) {
//   SDL_Color color = {255, 255, 255,
//                      255}; // White color for the FPS counter text
//   char fpsText[15];
//   sprintf(fpsText, "FPS: %d", fps); // Create a string for the FPS text

//   SDL_Surface *surface = TTF_RenderText_Solid(font, fpsText, color);
//   SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

//   SDL_Rect textRect = {5, 5, surface->w, surface->h}; // Position: top left
//   SDL_FreeSurface(surface);                           // No longer needed

//   SDL_RenderCopy(renderer, texture, NULL, &textRect);
//   SDL_DestroyTexture(texture); // Clean up
// }

void render_stat(SDL_Renderer *renderer, TTF_Font *font, char *s, int x,
                 int y) {

  SDL_Color color = {255, 255, 255, 255};

  SDL_Surface *surface = TTF_RenderText_Solid(font, s, color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect textRect = {x, y, surface->w, surface->h};
  SDL_FreeSurface(surface); // No longer needed

  SDL_RenderCopy(renderer, texture, NULL, &textRect);
  SDL_DestroyTexture(texture); // Clean up
}

mandelbrot_params m_params = {
    .focal_x = 0.0, .focal_y = 0.0, .zoom = 1.0, .max_iterations = 150};
mandelbrot_params rendered_m_params;

process_buffer p_buffer = {.progress = 0, .trigger_stop = 0, .done = 1};

// int INITIAL_RESOLUTION = 10;
// int MAX_RESOLUTION = 160;
float INITIAL_RESOLUTION = 1.0 / (32.0);
float current_resolution = 1.0 / (64.0);

double *mandel_data;
int max_mandel_rows = 1000;
int max_mandel_cols = 1000;

void rendering_setup() {
  mandel_data = malloc(max_mandel_cols * max_mandel_rows * sizeof(double));
}

// TODO this should be platfrom agnostic
void draw(Uint32 *pixels, int screenWidth, int screenHeight) {
  if (!mandelbrot_params_eq(&m_params, &rendered_m_params)) {
    current_resolution = INITIAL_RESOLUTION;
    rendered_m_params = m_params;
  } else if (current_resolution < 1.0) {
    // request to render something new
    current_resolution *= 2.0;
    if (p_buffer.done) {
      printf("compute... \n");
      mandelbrot_compute(mandel_data, screenWidth * current_resolution,
                         screenHeight * current_resolution, &m_params,
                         &p_buffer);

      if (p_buffer.trigger_stop) {
        printf("interrupted. skip rendering.");
        p_buffer.trigger_stop = 0;
      } else {
        render_data(mandel_data, screenWidth * current_resolution,
                    screenHeight * current_resolution, pixels, screenWidth,
                    screenHeight);
      }
    }
  }
}

void draw_text(SDL_Renderer *renderer, TTF_Font *font, int screenWidth,
               int screenHeight) {
  char focal_stat[64];

  sprintf(focal_stat, "%f, %fi @%f", m_params.focal_x, m_params.focal_y,
          m_params.zoom);
  render_stat(renderer, font, focal_stat, 5, 25);

  char resolution_stat[64];
  sprintf(resolution_stat, "[%d%%] - %d iterations", (p_buffer.progress),
          ((int)(m_params.max_iterations)));
  render_stat(renderer, font, resolution_stat, 5, 45);
}

void handle_keyboard_event(SDL_KeyboardEvent *key) {
  double MOVE_SENSITIVITY = .1;
  double move_nip = MOVE_SENSITIVITY / m_params.zoom;

  switch (key->keysym.sym) {
  case SDLK_LEFT:
    p_buffer.trigger_stop = 1;
    m_params.focal_x -= move_nip;
    break;
  case SDLK_RIGHT:
    p_buffer.trigger_stop = 1;
    m_params.focal_x += move_nip;
    break;
  case SDLK_DOWN:
    p_buffer.trigger_stop = 1;
    m_params.focal_y -= move_nip;
    break;
  case SDLK_UP:
    p_buffer.trigger_stop = 1;
    m_params.focal_y += move_nip;
    break;
  case SDLK_EQUALS:
  case SDLK_PLUS:
    p_buffer.trigger_stop = 1;
    m_params.zoom *= 1.1;
    break;
  case SDLK_MINUS:
    p_buffer.trigger_stop = 1;
    m_params.zoom /= 1.1;
    break;
  case SDLK_COMMA:
    if (m_params.max_iterations > 10) {

      p_buffer.trigger_stop = 1;
      m_params.max_iterations -= 10;
    }
    break;
  case SDLK_PERIOD:
    p_buffer.trigger_stop = 1;
    m_params.max_iterations += 10;
    break;
  default:
    break;
  }
}
