#ifndef MANDELBROT_H
#define MANDELBROT_H

struct MandelbrotParams {
  double focal_x;
  double focal_y;
  double zoom;
};

typedef struct MandelbrotParams mandelbrot_params;

int mandelbrot_params_eq(mandelbrot_params *a, mandelbrot_params *b); 
mandelbrot_params mandelbrot_params_new(double x, double y, double zoom);
void mandelbrot_render(Uint32 *pixels, int width, int height, mandelbrot_params *params, int max_iterations);
void mandelbrot_render_resolution(
    Uint32 *pixels,
    int width,
    int height,
    int resolution_width,
    int resolution_height,
    mandelbrot_params *params,
    int max_iterations);
#endif
