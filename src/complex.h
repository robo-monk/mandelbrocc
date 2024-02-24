#ifndef COMPLEX_H
#define COMPLEX_H

// Declaration of the ComplexNumber struct
struct ComplexNumber {
  double im;
  double re;
};

// Typedef for easier usage
typedef struct ComplexNumber complex;

// Function prototypes
void complex_print(complex *a);
complex complex_new(double re, double im);
complex complex_multiply(complex *a, complex *b);
complex complex_add(complex *a, complex *b);
double complex_magnitude(complex *a);
double complex_magnitude_lts(complex *x, double value);

#endif
