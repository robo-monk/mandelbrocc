#include <stdio.h>
#include "complex.h"

typedef struct ComplexNumber complex;

void complex_print(complex *a) {
  printf("%f  %fi\n", a->re, a->im);
}

complex complex_new(double re, double im) {
  complex a = { .im = im, .re = re };
  return a;
}

complex complex_multiply(complex *a, complex *b) {
  complex r = {
    .im = a->im * b->im,
    .re = a->re * b->re
  };

  return r;
}

complex complex_add(complex *a, complex *b) {
  complex r = {
    .im = a->im + b->im,
    .re = a->re + b->re
  };

  return r;
}
