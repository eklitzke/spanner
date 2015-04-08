#include "./particle.h"

void move_particles(std::vector<Particle> *particles,
                    double gravity,
                    double timescale) {
    for (Particle &p : *particles) {
      p.zero_force();
      for (const Particle &o : *particles) {
        p.augment_force(gravity, o);
      }
    }
    for (Particle &p : *particles) {
      p.augment_velocity(timescale);
      p.augment_position(timescale);
    }
}


void draw_particles(cairo_t *ctx, const std::vector<Particle> &particles) {
  for (const Particle &p : particles) {
      cairo_set_source_rgb(ctx, 0, 0, 0);
      cairo_arc(ctx,
                p.position().x(),
                p.position().y(),
                sqrt(p.mass()) * 0.03,
                0,
                2 * M_PI);
      cairo_set_line_width(ctx, 0.01);
      cairo_stroke_preserve(ctx);

      cairo_set_source_rgb(
          ctx,
          p.color().r(),
          p.color().g(),
          p.color().b());
      cairo_fill(ctx);
    }
}
