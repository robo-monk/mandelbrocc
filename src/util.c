#include <stdlib.h>
#include "util.h"

int get_rand(int lower, int upper)
{
  return (rand() % (upper - lower + 1)) + lower;
}
