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

TextUI focal_point_text, zoom_text, process_text, iterations_text;

void rendering_setup(int screenWidth, int screenHeight)
{
  free(mandel_data);
  mandel_data = malloc(screenHeight * screenWidth * max_mandel_rows * sizeof(double));

  focal_point_text = new_text_ui("%f, %fi", 5, 25);
  zoom_text = new_text_ui("zoom: %f", 5, 45);
  iterations_text = new_text_ui("%d iterations", 5, 65);
  process_text = new_text_ui("render@%.2f  %d%%", 5, 85);
}

TextUI new_text_ui(char *fmt_string, int x, int y)
{
  TextUI txt_ui = {
      .format_str = fmt_string, .x = x, .y = y};

  txt_ui.str = malloc(512 * sizeof(char));
  return txt_ui;
}

int hide_ui = 0;

void draw_text()
{
  if (hide_ui)
    return;

  __format_text_ui(&focal_point_text, m_params.focal_x,
                   m_params.focal_y);
  __format_text_ui(&zoom_text, m_params.zoom);

  __format_text_ui(&process_text, current_resolution,
                    p_buffer.progress);

  __format_text_ui(&iterations_text, m_params.max_iterations);

  __platform_render_text_ui(&focal_point_text);
  __platform_render_text_ui(&zoom_text);
  __platform_render_text_ui(&process_text);
  __platform_render_text_ui(&iterations_text);
}

void draw(color *pixels, int screenWidth, int screenHeight)
{
  if (!mandelbrot_params_eq(&m_params, &rendered_m_params))
  {
    current_resolution = INITIAL_RESOLUTION;
    rendered_m_params = m_params;
  }
  else if (current_resolution < 1.0)
  {

    current_resolution *= 2.0;
    if (p_buffer.done)
    {
      p_buffer.progress = 0;
      printf("(%f) compute... \n", current_resolution);
      mandelbrot_compute(mandel_data, screenWidth * current_resolution,
                         screenHeight * current_resolution, &m_params,
                         &p_buffer);
      printf("(%f) done computing... \n", current_resolution);
      if (p_buffer.trigger_stop)
      {
        printf("interrupted. skip rendering. \n");
        p_buffer.trigger_stop = 0;
      }
      else
      {
        render_data(mandel_data, screenWidth * current_resolution,
                    screenHeight * current_resolution, pixels, screenWidth,
                    screenHeight);
      }
    }
  }
}

void perform_action(Action action)
{
  if (action == NOP)
    return;
  double MOVE_SENSITIVITY = .05;
  double move_nip = MOVE_SENSITIVITY / m_params.zoom;

  switch (action)
  {
  case ACTION_LEFT:
    p_buffer.trigger_stop = 1;
    m_params.focal_x -= move_nip;
    break;
  case ACTION_RIGHT:
    p_buffer.trigger_stop = 1;
    m_params.focal_x += move_nip;
    break;
  case ACTION_DOWN:
    p_buffer.trigger_stop = 1;
    m_params.focal_y += move_nip;
    break;
  case ACTION_UP:
    p_buffer.trigger_stop = 1;
    m_params.focal_y -= move_nip;
    break;
  case ACTION_ZOOM_IN:
    p_buffer.trigger_stop = 1;
    m_params.zoom *= 1.1;
    break;
  case ACTION_ZOOM_OUT:
    p_buffer.trigger_stop = 1;
    m_params.zoom /= 1.1;
    break;
  case MAX_ITERATIONS_DECR:
    if (m_params.max_iterations > 10)
    {
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
