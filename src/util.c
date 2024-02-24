#include "util.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int get_rand(int lower, int upper) {
  return (rand() % (upper - lower + 1)) + lower;
}

int clamp(int value, int min, int max) {
  if (value < min)
    return min;
  if (value > max)
    return max;
  return value;
}

// absolutes the array and
void abs_min_max(double *data, int len, double *min, double *max) {
  for (int i = 0; i < len; i++) {
    data[i] = fabs(data[i]);
    double curr_val = data[i];
    if (curr_val > *max) {
      *max = data[i];
    }
    if (curr_val < *min) {
      *min = data[i];
    }
  }
}

void abs_normalize(double *data, int len, double low_bound, double high_bound) {
  double min = 0.0;
  double max = 0.0;
  abs_min_max(data, len, &min, &max);

  double diff = high_bound - low_bound;
  double diff_array = max - min;

  for (int i = 0; i < len; i++) {
    double curr_val = data[i];
    double temp = (((data[i] - min) * diff) / diff_array) + low_bound;
    data[i] = temp;
  }
}
