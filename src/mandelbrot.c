#include <stdio.h>
#include "SDL.h"
#include "complex.h"
#include "mandelbrot.h"

mandelbrot_params mandelbrot_params_new(double x, double y, double zoom)
{
    mandelbrot_params p = {
        .focal_x = x,
        .focal_y = y,
        .zoom = zoom};
    return p;
}

int mandelbrot_params_eq(mandelbrot_params *a, mandelbrot_params *b)
{
    return (a->focal_x == b->focal_x) && (a->focal_y == b->focal_y) && (a->zoom == b->zoom);
}

double run_mandelbrot(complex *c)
{
    complex z = complex_new(0.0, 0.0);
    int i = 0;
    int MAX_ITERATIONS = 200;
    while (i < MAX_ITERATIONS)
    {
        complex r = complex_multiply(&z, &z);
        z = complex_add(c, &r);

        if (complex_magnitude(&z) > 4.0)
        {
            break;
        }

        i++;
    }
    return (double)i / (double)MAX_ITERATIONS;
}

Uint32 rgb(int r, int g, int b)
{
    Uint32 col = (255 << 24) + (r << 16) + (g << 8) + b; // Assuming ARGB8888 format
    return col;
}

// fn calculate_bounds(focal_point : Complex<f64>, zoom : f64, aspect_ratio : f64)->(f64, f64, f64, f64)
// {
//     let min_x = focal_point.re - (aspect_ratio / zoom);
//     let max_x = focal_point.re + (aspect_ratio / zoom);
//     let min_y = focal_point.im - (1.0 / zoom);
//     let max_y = focal_point.im + (1.0 / zoom);
//     (min_x, max_x, min_y, max_y)
// }

void calc_bounds(complex *min_c, complex *max_c, complex *focal_point, double zoom, double aspect_ratio)
{
    min_c->re = focal_point->re - (aspect_ratio / zoom);
    min_c->im = focal_point->im - (1.0 / zoom);

    max_c->re = focal_point->re + (aspect_ratio / zoom);
    max_c->im = focal_point->im + (1.0 / zoom);
}

double map_value(double value, double start1, double stop1, double start2, double stop2)
{
    return (value - start1) / (stop1 - start1) * (stop2 - start2) + start2;
}

void mandelbrot_render(
    Uint32 *pixels,
    int width,
    int height,
    mandelbrot_params *params)
{
    double aspect_ratio = width / height;

    complex min_c = complex_new(0.0, 0.0);
    complex max_c = complex_new(0.0, 0.0);
    complex focal_point = complex_new(params->focal_x, params->focal_y);

    calc_bounds(&min_c, &max_c, &focal_point, params->zoom, aspect_ratio);

    double x_incr = (max_c.re - min_c.re) / (double)width;
    double y_incr = (max_c.im - min_c.im) / (double)height;

    int i = 0;
    for (double y = min_c.im; y < max_c.im; y += y_incr)
    {
        for (double x = min_c.re; x < max_c.re; x += x_incr)
        {
            complex current_point = complex_new(x, y);
            double c = 255 * run_mandelbrot(&current_point);
            pixels[i] = rgb(c, c, c);
            i++;
        }
    }
}
