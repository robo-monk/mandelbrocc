#include <stdio.h>
#include "SDL.h"
#include "complex.h"
#include "mandelbrot.h"

mandelbrot_params mandelbrot_params_new(double x, double y, double zoom) {
    mandelbrot_params p = {
        .focal_x = x,
        .focal_y = y,
        .zoom = zoom
    };
    return p;
}


int mandelbrot_params_eq(mandelbrot_params *a, mandelbrot_params *b) {
    return (a->focal_x == b->focal_x)
        && (a->focal_y == b->focal_y) 
        && (a->zoom    == b->zoom);
}

double run_mandelbrot(complex *c) {
    complex z = complex_new(0.0, 0.0);
    int i = 0;
    int MAX_ITERATIONS = 200;
    while (i < MAX_ITERATIONS) {
        complex r = complex_multiply(&z, &z);
        z = complex_add(c, &r);

        if (complex_magnitude(&z) > 4.0) {
            break;
        }

        i++;
    }
    return (double) i/ (double) MAX_ITERATIONS;
}

Uint32 rgb(int r, int g, int b) {
    Uint32 col = (255 << 24) + (r << 16) + (g << 8) + b; // Assuming ARGB8888 format
    return col;
}

void mandelbrot_render(
    Uint32 *pixels,
    int width,
    int height,
    mandelbrot_params *params
) {
    complex focal_point = complex_new(params->focal_x, params->focal_y);

    for (int x = 0; x < width; x += 1) {
        for (int y = 0; y < height; y += 1) {
            double map_x = (x-width/2.0)/ (double) 100.0;
            double map_y = (y-height/2.0)/ (double) 100.0;
            complex current_point = complex_new(map_x, map_y);
            double c = 255*run_mandelbrot(&current_point);
            pixels[(y * width) + x] = rgb(c, c, c);
        }
    }
}
