#include "complex.h"
#include <math.h>
#include <stdio.h>

typedef struct ComplexNumber complex;

void complex_print(complex *a) { printf("%f  %fi\n", a->re, a->im); }

complex complex_new(double re, double im) {
  complex a = {.im = im, .re = re};
  return a;
}

complex complex_multiply(complex *x, complex *y) {
  //   (a + bi) * (c + di)
  // = (ac + adi) + (bci - bd) =
  // = (ac - bd) + (ad + bc)i

  //   a = x.re, c = y.re
  //   b = x.im, d = y.im
  complex r = {.re = (x->re * y->re - x->im * y->im),
               .im = (x->re * y->im + x->im * y->re)};

  return r;
}

complex complex_add(complex *a, complex *b) {
  complex r = {.im = a->im + b->im, .re = a->re + b->re};

  return r;
}

double complex_magnitude(complex *x) {
  return sqrt(pow(x->re, 2) + pow(x->im, 2));
}

double complex_magnitude_lts(complex *x, double value) {
  return (pow(x->re, 2) + pow(x->im, 2)) > value;
}
