#include <stdlib.h>
#include "util.h"

int get_rand(int lower, int upper)
{
  return (rand() % (upper - lower + 1)) + lower;
}

int clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
