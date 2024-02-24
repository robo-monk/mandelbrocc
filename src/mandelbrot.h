#ifndef MANDELBROT_H
#define MANDELBROT_H

struct MandelbrotParams {
  double focal_x;
  double focal_y;
  double zoom;
  unsigned int max_iterations;
};

struct ProcessBuffer {
  int trigger_stop;
  int progress;
  int done;
};

typedef struct ProcessBuffer process_buffer;
typedef struct MandelbrotParams mandelbrot_params;

int mandelbrot_params_eq(mandelbrot_params *a, mandelbrot_params *b);
mandelbrot_params mandelbrot_params_new(double x, double y, double zoom);

void mandelbrot_compute(double *data, int rows, int cols,
                        mandelbrot_params *params, process_buffer *process);
#endif
