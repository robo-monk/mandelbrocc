#include <stdio.h>
#include "complex.h"
#include "application.h"

int main() {
  complex a = { .im = 0.0, .re = 0.0 };
  complex b = { .im = -1.0, .re = +2.0 };
  complex r = complex_add(&a, &b);
  complex_print(&r);
	printf("Hello world!");

  init_sdl();
	return 0;
}
