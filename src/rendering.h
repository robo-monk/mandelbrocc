#ifndef RENDERING_H
#define RENDERING_H

#include "frontend.h"
void rendering_setup(int screenWidth, int screenHeight);
void draw(unsigned int *pixels, int screenWidth, int screenHeight);
void perform_action(Action action);

#endif
