#include <stdio.h>

struct ComplexNumber {
  double im;
  double re;
};

typedef struct ComplexNumber complex;

void complex_print(complex *a) {
  printf("%f  %fi\n", a->re, a->im);
}

complex complex_new(double re, double im) {
  complex a = { .im = im, .re = re };
  return a;
}

complex complex_add(complex *a, complex *b) {
  complex r = {
    .im = a->im + b->im,
    .re = a->re + b->re
  };

  return r;
}

int main() {
  complex a = { .im = 0.0, .re = 0.0 };
  complex b = { .im = -1.0, .re = +2.0 };
  complex r = complex_add(&a, &b);
  complex_print(&r);
	printf("Hello world!");
	return 0;
}
