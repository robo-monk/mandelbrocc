#ifndef FRONTEND_H
#define FRONTEND_H

typedef enum {
  NOP,

  ACTION_LEFT,
  ACTION_RIGHT,
  ACTION_DOWN,
  ACTION_UP,

  ACTION_ZOOM_IN,
  ACTION_ZOOM_OUT,

  MAX_ITERATIONS_INCR,
  MAX_ITERATIONS_DECR,

  HIDE_UI,
  SCREENSHOT
} Action;

typedef struct {
  char *format_str;
  char *str;
  int x;
  int y;
} TextUI;

TextUI new_text_ui(char *fmt_string, int x, int y);

typedef unsigned int color;

// typedef void (*PlatformRenderTextFn)(TextUI *text_ui, ...);

color rgb(int r, int g, int b);

void render_data(double *data, int data_rows, int data_cols, color *pixels,
                 int image_width, int image_height);

void __format_text_ui(TextUI *text_ui, ...);
void __platform_render_text_ui(TextUI *text_ui);
#endif
