#include "utils.h"

int clamp_uint32(int d, int min, int max) {
  const int t = d < min ? min : d;
  return t > max ? max : t;
}