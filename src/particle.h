// -*- C++ -*-

#ifndef SPANNER_PARTICLE_H_
#define SPANNER_PARTICLE_H_

#include <cairo/cairo.h>
#include <eigen3/Eigen/Dense>

#include <iostream>

#include "./color.h"

typedef Eigen::Vector2d Vec;

class Particle {
 public:
  Particle() = delete;
  Particle(const Vec &position, const Vec &velocity, double mass)
      :position_(position),
       velocity_(velocity),
       mass_(mass) {}
  Particle(const Vec &position, const Vec &velocity, double mass, Color color)
      :position_(position),
       velocity_(velocity),
       mass_(mass),
       color_(color) {}

  inline const Vec& position() const { return position_; }
  inline const Vec& velocity() const { return velocity_; }
  inline const Vec& force() const { return force_; }

  inline const Color& color() const { return color_; }

  inline double mass() const { return mass_; }
  inline double momentum() const {
    return mass_ * velocity_.norm();
  }

  void zero_force() {
    force_ = Vec::Zero();
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
  Color color_;
};


void move_particles(std::vector<Particle> *particles,
                    double gravity,
                    double timescale);

void draw_particles(cairo_t *ctx,
                    const std::vector<Particle> &particles,
                    double scale_factor);


#endif  // SPANNER_PARTICLE_H_
