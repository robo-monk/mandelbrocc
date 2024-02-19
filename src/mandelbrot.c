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

double run_mandelbrot(complex *c, int max_iterations)
{
    complex z = complex_new(0.0, 0.0);
    int i = 0;
    while (i < max_iterations)
    {
        complex r = complex_multiply(&z, &z);
        z = complex_add(c, &r);

        if (complex_magnitude(&z) > 4.0)
        {
            break;
        }

        i++;
    }
    return (double)i / (double)max_iterations;
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

// int map_value(int value, int start1, int stop1, int start2, int stop2)
// {
//     return (value - start1) / (stop1 - start1) * (stop2 - start2) + start2;
// }



// maps image a to b
void image_map(
    Uint32 *image_a,
    int width_a,
    int height_a,

    Uint32 *image_b,
    int width_b,
    int height_b)
{
    double x_ratio = width_a / width_b;
    double y_ratio = height_a / height_b;

    for (int x = 0; x < width_b; x++)
    {
        for (int y = 0; y < height_b; y++)
        {
            int i_a = x * x_ratio + y * y_ratio * height_a;
            int i_b = x + y * height_b;
            image_b[i_b] = image_a[i_a];
        }
    }
}

void mandelbrot_render_resolution(
    Uint32 *pixels,
    int width,
    int height,
    int resolution_width,
    int resolution_height,
    mandelbrot_params *params,
    int max_iterations)
{
    Uint32 *r_image = malloc(resolution_width * resolution_height * sizeof(Uint32));
    mandelbrot_render(r_image, resolution_width, resolution_height, params, max_iterations);
    image_map(r_image, resolution_width, resolution_height, pixels, width, height);
    free(r_image);
}

enum PixelMergeType {
    PIXEL_AVERAGE
};

typedef enum PixelMergeType Pixel_merge_type;

void render_data(
    double *data,
    int data_rows, int data_cols,

    Uint32 *pixels,
    int image_width, int image_height
) {
    // merging 
    Pixel_merge_type pixelMergeType = PIXEL_AVERAGE;
    int data_index = 0;

    double aspect_ratio = image_width / image_height;
    // for every pixel of the image how much should the index move?
    // if we have 30 vertical points and 300 pixels, the index should move 30/300 -> 0.1
    // in the future for index 23.1 for example we can do 90% of the index 23 value and 10% of the index 24
    // double x_incr = (max_c.re - min_c.re) / ((double)rows);
    // double y_incr = (max_c.im - min_c.im) / ((double)cols);

    double data_x_incr = (double) data_rows / (double) image_width;
    double data_y_incr = (double) data_cols / (double) image_height;

    double data_x = 0;
    double data_y = 0;

    for (double y = 0; y < image_width; y += 1)
    {
        for (double x = 0; x < image_height; x += 1)
        {
            // find the nearest datapoint
            int image_index = y * image_height + x;
            // int data_index = (int) map_value(image_index, 0, image_width * image_height, 0, data_rows * data_cols);
            int data_index = ((int) data_x) + (int) data_y * data_cols;

            double value = 200*data[data_index];
            pixels[image_index] = rgb(value, value, value);

            data_x += data_x_incr;
        }
        data_x = 0;
        data_y += data_y_incr;
    }
}


void mandelbrot_compute(
    double *data,
    int rows,
    int cols,
    mandelbrot_params *params,
    int max_iterations)
{
    double aspect_ratio = rows / cols;
    complex min_c = complex_new(0.0, 0.0);
    complex max_c = complex_new(0.0, 0.0);
    complex focal_point = complex_new(params->focal_x, params->focal_y);

    calc_bounds(&min_c, &max_c, &focal_point, params->zoom, aspect_ratio);

    double x_incr = (max_c.re - min_c.re) / ((double) rows);
    double y_incr = (max_c.im - min_c.im) / ((double) cols);

    int image_x = 0;
    int image_y = 0;
    for (double y = min_c.im; y < max_c.im; y += y_incr)
    {
        for (double x = min_c.re; x < max_c.re; x += x_incr)
        {
            complex current_point = complex_new(x, y);
            double c = run_mandelbrot(&current_point, max_iterations);
            image_x += 1;
            data[(image_y * cols) + image_x] = c;
        }
        image_x = 0;
        image_y += 1;
    }
}

// TODO instead of pasting on the actual pixels,
// map a width x height pixels array on the Uint32 pixels
void mandelbrot_render(
    Uint32 *pixels,
    int width,
    int height,
    mandelbrot_params *params,
    int max_iterations)
{
    float resolution = 0.2;
    double aspect_ratio = width / height;

    complex min_c = complex_new(0.0, 0.0);
    complex max_c = complex_new(0.0, 0.0);
    complex focal_point = complex_new(params->focal_x, params->focal_y);

    calc_bounds(&min_c, &max_c, &focal_point, params->zoom, aspect_ratio);

    double x_incr = (max_c.re - min_c.re) / ((double)width * resolution);
    double y_incr = (max_c.im - min_c.im) / ((double)height * resolution);

    // int i = 0;
    int image_x = 0;
    int image_y = 0;
    for (double y = min_c.im; y < max_c.im; y += y_incr)
    {
        for (double x = min_c.re; x < max_c.re; x += x_incr)
        {
            complex current_point = complex_new(x, y);
            double c = 200 * run_mandelbrot(&current_point, max_iterations);
            image_x += 1 / resolution;
            pixels[(image_y * height) + image_x] = rgb(c, c, c);
        }
        image_x = 0;
        image_y += 1 / resolution;
    }
}
