#include "sdl_frontend.h"
#include "SDL.h"

Uint32 rgb(int r, int g, int b)
{
    Uint32 col = (255 << 24) + (r << 16) + (g << 8) + b; // Assuming ARGB8888 format
    return col;
}

int clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

Uint32 convert_to_col(double *data, double x, double y, int data_rows, int data_cols) {
    int x1 = (int) x;
    int y1 = (int) y;

    int x2 = clamp(x1 + 1, 0, data_rows - 1);
    int y2 = clamp(y1 + 1, 0, data_cols - 1);

    double fx = x - x1;
    double fy = y - y1;

    double value =
        (1 - fx) * (1 - fy) * data[y1 * data_cols + x1] +
        fx * (1 - fy) * data[y1 * data_cols + x2] +
        (1 - fx) * fy * data[y2 * data_cols + x1] +
        fx * fy * data[y2 * data_cols + x2];
    
    return rgb(255*value, 255*value, 255*value);
}

void render_data(
    double *data,
    int data_rows, int data_cols,

    Uint32 *pixels,
    int image_width, int image_height
) {
    // merging 
    int data_index = 0;

    double aspect_ratio = image_width / image_height;
    // for every pixel of the image how much should the index move?
    // if we have 30 vertical points and 300 pixels, the index should move 30/300 -> 0.1
    // in the future for index 23.1 for example we can do 90% of the index 23 value and 10% of the index 24

    double data_x_incr = (double) data_rows / (double) image_width;
    double data_y_incr = (double) data_cols / (double) image_height;

    double data_x = 0;
    double data_y = 0;

    for (double y = 0; y < image_width; y += 1)
    {
        for (double x = 0; x < image_height; x += 1)
        {
            int image_index = y * image_height + x;

            long data_x_index = (long) data_x;
            double data_x_fractional = data_x - data_x_index;

            long data_y_index = (long) data_y;
            double data_y_fractional = data_y - data_y_index;

            // TODO find 4 surrounding pixels
            // int data_index1 = ((long) data_x) + (long) data_y * data_cols;
            // int data_index2 = ((long) data_x + 1) + (long) (data_y) * data_cols;
            // int data_index3 = ((long) data_x + 1) + (long) (data_y+1) * data_cols;
            // double value = 255*(data[data_index1]*(1.0-(data_x_fractional+data_y_fractional)) + data[data_index2]*(data_x_fractional) + data[data_index3]*(data_y_fractional));


            // int data_index = ((int) data_x) + (int) data_y * data_cols;
            // double value = 200*data[data_index];
            // pixels[image_index] = rgb(value, value, value);
            pixels[image_index] = convert_to_col(data, data_x, data_y, data_rows, data_cols);

            data_x += data_x_incr;
        }
        data_x = 0;
        data_y += data_y_incr;
    }
}