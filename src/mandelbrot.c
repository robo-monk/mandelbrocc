#include <stdio.h>
#include "SDL.h"
#include "complex.h"

double run_mandelbrot(complex *c) {
    complex z = complex_new(0.0, 0.0);
    int i = 0;
    int MAX_ITERATIONS = 100;
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
    double focal_x,
    double focal_y,
    double zoom
) {
    complex focal_point = complex_new(focal_x, focal_y);

    for (int x = 0; x < width; x += 1) {
        for (int y = 0; y < height; y += 1) {
            double map_x = (x-width/2.0)/ (double) 100.0;
            double map_y = (y-height/2.0)/ (double) 100.0;
            complex current_point = complex_new(map_x, map_y);
            double c = 100*run_mandelbrot(&current_point);
            pixels[(y * width) + x] = rgb(c, c, c);
        }
    }
}
