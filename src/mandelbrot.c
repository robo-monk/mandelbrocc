#include "mandelbrot.h"
#include "complex.h"

mandelbrot_params mandelbrot_params_new(double x, double y, double zoom) {
  mandelbrot_params p = {.focal_x = x, .focal_y = y, .zoom = zoom};
  return p;
}

int mandelbrot_params_eq(mandelbrot_params *a, mandelbrot_params *b) {
  return (a->focal_x == b->focal_x) && (a->focal_y == b->focal_y) &&
         (a->zoom == b->zoom) && (a->max_iterations == b->max_iterations);
}

double run_mandelbrot(complex *c, int max_iterations) {
  complex z = complex_new(0.0, 0.0);
  int i = 0;
  while (i < max_iterations) {
    complex r = complex_multiply(&z, &z);
    z = complex_add(c, &r);

    if (complex_magnitude_lts(&z, 4.0)) {
      break;
    }

    i++;
  }
  return (double)i / (double)max_iterations;
}

void calc_bounds(complex *min_c, complex *max_c, complex *focal_point,
                 double zoom, double aspect_ratio) {
  min_c->re = focal_point->re - (aspect_ratio / zoom);
  min_c->im = focal_point->im - (1.0 / zoom);

  max_c->re = focal_point->re + (aspect_ratio / zoom);
  max_c->im = focal_point->im + (1.0 / zoom);
}

void mandelbrot_compute(double *data, int rows, int cols,
                        mandelbrot_params *params) {
  double aspect_ratio = (double)rows / cols;
  complex min_c = complex_new(0.0, 0.0);
  complex max_c = complex_new(0.0, 0.0);
  complex focal_point = complex_new(params->focal_x, params->focal_y);

  calc_bounds(&min_c, &max_c, &focal_point, params->zoom, aspect_ratio);

  double x_incr = (max_c.re - min_c.re) / ((double)rows);
  double y_incr = (max_c.im - min_c.im) / ((double)cols);

  int image_x = 0;
  int image_y = 0;

  for (double y = min_c.im; y < max_c.im; y += y_incr) {
    for (double x = min_c.re; x < max_c.re; x += x_incr) {
      complex current_point = complex_new(x, y);
      double c = run_mandelbrot(&current_point, params->max_iterations);
      image_x += 1;
      data[(image_y * cols) + image_x] = c;
    }
    image_x = 0;
    image_y += 1;
  }
}
