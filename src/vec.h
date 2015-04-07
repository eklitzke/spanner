// -*- C++ -*-

#ifndef SPANNER_VEC_H_
#define SPANNER_VEC_H_

#include <math.h>

#include <iostream>

class Vec {
 public:
  Vec() :x_(0), y_(0) {}
  Vec(double x, double y) :x_(x), y_(y) {}

  inline double x() const { return x_; }
  inline double y() const { return y_; }

  inline double dist_sq(const Vec &other) const {
    double dx = x_ - other.x_;
    double dy = y_ - other.y_;
    return dx * dx + dy * dy;
  }

  inline double dist(const Vec &other) const {
    return sqrt(dist_sq(other));
  }

  inline void scale(double factor) {
    x_ *= factor;
    y_ *= factor;
  }

  inline void zero() {
    x_ = 0;
    y_ = 0;
  }

  inline void augment(const Vec &other) {
    x_ += other.x_;
    y_ += other.y_;
  }

  inline bool is_zero() const {
    return x_ == 0 && y_ == 0;
  }

  inline Vec difference(const Vec &other) const {
    return Vec(x_ - other.x_, y_ - other.y_);
  }

 private:
  double x_;
  double y_;
};


std::ostream& operator<<(std::ostream &os, const Vec &v) {
  os << '(' << v.x() << ", " << v.y() << ')';
  return os;
}




#endif  // SPANNER_VEC_H_
