#ifndef UTILITY_H
#define UTILITY_H

int get_rand(int lower, int upper);
int clamp(int value, int min, int max);
void abs_normalize(double *data, int len, double low_bound, double high_bound);

#endif
