// -*- C++ -*-

#ifndef SPANNER_PARTICLE_H_
#define SPANNER_PARTICLE_H_

#include <eigen3/Eigen/Dense>

#include <iostream>


typedef Eigen::Vector2d Vec;

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
    force_ -= force_;
  }

  void augment_force(double g, const Particle &other) {
    Vec heading = other.position_ - position_;
    if (heading.isZero()) {
      return;
    }
    double magnitude = g * mass_ * other.mass_ / heading.squaredNorm();
    heading *= magnitude;
    force_ += heading;
  }

  void augment_velocity(double timescale) {
    force_ *= timescale;
    velocity_ += force_;
  }

  void augment_position(double timescale) {
    position_ += (velocity_ * timescale);
  }

 private:
  Vec position_;
  Vec velocity_;
  Vec force_;
  double mass_;
};

#endif  // SPANNER_PARTICLE_H_
