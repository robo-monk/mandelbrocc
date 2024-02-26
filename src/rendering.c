#include "rendering.h"
#include "mandelbrot.h"
#include <stdio.h>
#include <stdlib.h>

mandelbrot_params m_params = {
    .focal_x = 0.0, .focal_y = 0.0, .zoom = 1.0, .max_iterations = 150};
mandelbrot_params rendered_m_params;

process_buffer p_buffer = {.progress = 0, .trigger_stop = 0, .done = 1};

float INITIAL_RESOLUTION = 1.0 / (32.0);
float current_resolution = 1.0 / (64.0);

double *mandel_data;
int max_mandel_rows = 1000;
int max_mandel_cols = 1000;

char *text;

void rendering_setup(int screenWidth, int screenHeight) {
  free(mandel_data);
  mandel_data = malloc(max_mandel_cols * max_mandel_rows * sizeof(double));
}

int hide_ui = 0;
TextUI focal_point_text = {.format_str = "%f, %fi", .x = 5, .y = 25};

TextUI zoom_text = {.format_str = "zoom: %f", .x = 5, .y = 55};

TextUI process_text = {
    .format_str = "%d iterations -- [%d%%]", .x = 5, .y = 85};

void draw_text() {
  if (hide_ui)
    return;

  __platform_render_text_ui(&focal_point_text, m_params.focal_x,
                            m_params.focal_y);
  __platform_render_text_ui(&zoom_text, m_params.zoom);
  __platform_render_text_ui(&process_text, m_params.max_iterations,
                            p_buffer.progress);
}

void draw(color *pixels, int screenWidth, int screenHeight) {
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

  draw_text();
}

void perform_action(Action action) {
  if (action == NOP)
    return;
  double MOVE_SENSITIVITY = .1;
  double move_nip = MOVE_SENSITIVITY / m_params.zoom;

  switch (action) {
  case LEFT:
    p_buffer.trigger_stop = 1;
    m_params.focal_x -= move_nip;
    break;
  case RIGHT:
    p_buffer.trigger_stop = 1;
    m_params.focal_x += move_nip;
    break;
  case DOWN:
    p_buffer.trigger_stop = 1;
    m_params.focal_y -= move_nip;
    break;
  case UP:
    p_buffer.trigger_stop = 1;
    m_params.focal_y += move_nip;
    break;
  case ZOOM_IN:
    p_buffer.trigger_stop = 1;
    m_params.zoom *= 1.1;
    break;
  case ZOOM_OUT:
    p_buffer.trigger_stop = 1;
    m_params.zoom /= 1.1;
    break;
  case MAX_ITERATIONS_DECR:
    if (m_params.max_iterations > 10) {
      p_buffer.trigger_stop = 1;
      m_params.max_iterations -= 10;
    }
    break;
  case MAX_ITERATIONS_INCR:
    p_buffer.trigger_stop = 1;
    m_params.max_iterations += 10;
    break;
  default:
    break;
  }
}
