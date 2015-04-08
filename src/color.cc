#include "./color.h"

#include <cmath>

// inspired by http://www.cs.rit.edu/~ncs/color/t_convert.html
Color HSVtoColor(float h, float s, float v) {
  if (s == 0) {
    return Color(v, v, v);
  }
  h /= 60;  // sector 0 to 5
  int i = floor(h);
  float f = h - i;  // factorial part of h
  float p = v * (1 - s );
  float q = v * (1 - s * f);
  float t = v * (1 - s * (1 - f));
  switch (i) {
    case 0:
      return Color(v, t, p);
      break;
    case 1:
      return Color(q, v, p);
      break;
    case 2:
      return Color(p, v, t);
      break;
    case 3:
      return Color(p, q, v);
      break;
    case 4:
      return Color(t, p, v);
      break;
    default:  // case 5
      return Color(v, p, q);
      break;
  }
}
