// -*- C++ -*-

#ifndef SPANNER_PARTICLE_H_
#define SPANNER_PARTICLE_H_

#include "./vec.h"

#include <iostream>

class Particle {
 public:
  Particle() = delete;
  Particle(const Vec &position, const Vec &velocity, double mass)
      :position_(position),
       velocity_(velocity),
       mass_(mass) {}

  inline const Vec& position() const { return position_; }
  inline const Vec& velocity() const { return velocity_; }
  inline const Vec& force() const { return force_; }

  inline double mass() const { return mass_; }

  void zero_force() {
    force_.zero();
  }

  void augment_force(double g, const Particle &other) {
    Vec heading = other.position_.difference(position_);
    if (heading.is_zero()) {
      return;
    }
    double magnitude = (
        g * mass_ * other.mass_ / position_.dist_sq(other.position_));
    heading.scale(magnitude);
    force_.augment(heading);
  }

  void augment_velocity(double timescale) {
    force_.scale(timescale);
    velocity_.augment(force_);
  }

  void augment_position(double timescale) {
    position_.augment({velocity_.x() * timescale, velocity_.y() * timescale});
  }

 private:
  Vec position_;
  Vec velocity_;
  Vec force_;
  double mass_;
};

#endif  // SPANNER_PARTICLE_H_
