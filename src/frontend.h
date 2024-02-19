#ifndef FRONTEND_H
#define FRONTEND_H

typedef unsigned int color;

color rgb(int r, int g, int b);

color convert_to_col(double *data, double x, double y, int data_rows, int data_cols);

void render_data(
    double *data,
    int data_rows, int data_cols,
    color *pixels,
    int image_width, int image_height);

#endif
