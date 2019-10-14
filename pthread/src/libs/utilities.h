#ifndef __UTILITIES_H_
#define __UTILITIES_H_

/* DO NOT OPTIMIZE THIS FILE */

double clampDouble(double const a, double const min, double const max) {
  if (a < min)
    return min;
  if (a > max)
    return max;
  return a;
}

unsigned int clampUInt(unsigned int const a, unsigned int const min, unsigned int const max) {
  if (a < min)
    return min;
  if (a > max)
    return max;
  return a;
}

int clampInt(int const a, int const min, int const max) {
  if (a < min)
    return min;
  if (a > max)
    return max;
  return a;
}

#endif // __UTILITIES_H_
