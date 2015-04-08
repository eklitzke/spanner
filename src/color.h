// -*- C++ -*-

#ifndef SPANNER_COLOR_H_
#define SPANNER_COLOR_H_

class Color {
 public:
  Color() :red_(1), green_(1), blue_(1) {}
  Color(float r, float g, float b) :red_(r), green_(g), blue_(b) {}

  inline float r() const { return red_; }
  inline float g() const { return green_; }
  inline float b() const { return blue_; }

 private:
  float red_;
  float green_;
  float blue_;
};

Color HSVtoColor(float h, float s, float v);

#endif  // SPANNER_COLOR_H_
