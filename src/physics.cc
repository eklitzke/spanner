#include "./physics.h"

#include <random>

std::vector<Particle> create_world(int num_particles) {
  std::vector<Particle> particles;

  std::random_device rd;
  std::mt19937 engine(rd());
  std::uniform_real_distribution<double> dist(-1, 1);

  float h = 0;
  float hue_increment = 360.f / num_particles;

  Vec pos_sum = Vec::Zero();
  Vec velocity_sum = Vec::Zero();
  for (int i = 0; i < num_particles - 1; i++) {
    Particle p(
        {2 * dist(engine), 2 * dist(engine)},
        {2 * dist(engine), 2 * dist(engine)},
        1,
        HSVtoColor(h, 1, 1));

    h += hue_increment;
    pos_sum += p.position();
    velocity_sum += p.velocity();
    particles.push_back(p);
  }

  Particle p(
      -pos_sum,
      -velocity_sum,
      1,
      HSVtoColor(h, 1, 1));
  particles.push_back(p);
  return particles;
}
