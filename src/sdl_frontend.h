#ifndef SDL_FRONT_H
#define SDL_FRONT_H

#include "SDL.h"

void render_data(
    double *data,
    int data_rows, int data_cols,

    Uint32 *pixels,
    int image_width, int image_height
);


#endif
