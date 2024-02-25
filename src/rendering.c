#include "rendering.h"
#include "frontend.h"
#include "mandelbrot.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdarg.h>
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

char *text;

typedef struct {
  char *format_str;
  int x;
  int y;
} TextUI;

SDL_Renderer *renderer;
TTF_Font *font;

void render_text_ui(TextUI *text_ui, ...) {
  char str[256];
  va_list args;
  va_start(args, text_ui->format_str);
  vsnprintf(str, sizeof(str), text_ui->format_str, args);
  va_end(args);

  render_stat(renderer, font, str, text_ui->x, text_ui->y);
}

void rendering_setup(int screenWidth, int screenHeight,
                     SDL_Renderer *sdl_renderer, TTF_Font *ttf_font) {
  free(mandel_data);
  free(text);
  mandel_data = malloc(max_mandel_cols * max_mandel_rows * sizeof(double));

  // TODO: review this
  text = malloc(((max_mandel_cols * max_mandel_rows) / 10) * sizeof(char));

  renderer = sdl_renderer;
  font = ttf_font;
}

// TODO this should be platfrom agnostic
void draw(Uint32 *pixels, int screenWidth, int screenHeight) {
  if (!mandelbrot_params_eq(&m_params, &rendered_m_params)) {
    current_resolution = INITIAL_RESOLUTION;
    rendered_m_params = m_params;
  } else if (current_resolution < 1.0) {

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

TextUI focal_point_text = {.format_str = "%f, %fi", .x = 5, .y = 25};

TextUI zoom_text = {.format_str = "zoom: %f", .x = 5, .y = 55};

TextUI process_text = {.format_str = "[%d%%] - %d iterations", .x = 5, .y = 85};

void draw_text(SDL_Renderer *renderer, TTF_Font *font, int screenWidth,
               int screenHeight) {

  render_text_ui(&focal_point_text, m_params.focal_x, m_params.focal_y);
  render_text_ui(&zoom_text, m_params.zoom);
  render_text_ui(&process_text, p_buffer.progress, m_params.max_iterations);
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
