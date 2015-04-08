#include <sys/time.h>
#include <time.h>

#include <boost/program_options.hpp>

#include <iostream>
#include <vector>

#include "../config.h"
#include "./particle.h"
#include "./physics.h"
#include "./x11.h"

namespace po = boost::program_options;


// scale the entire display
void scale(cairo_t *ctx, int window_size, double scale_factor) {
  cairo_identity_matrix(ctx);  // reset the CTM
  const double s = window_size / (scale_factor * 2);
  cairo_scale(ctx, s, s);
  cairo_translate(ctx, scale_factor, scale_factor);
}


// draw the crosshair
void draw_crosshair(cairo_t *ctx) {
    cairo_set_line_width(ctx, 0.05);
    cairo_set_source_rgb(ctx, 0, 0, 0);
    cairo_move_to(ctx, 0, 0.3);
    cairo_line_to(ctx, 0, -0.3);
    cairo_stroke(ctx);
    cairo_move_to(ctx, -0.3, 0);
    cairo_line_to(ctx, 0.3, 0);
    cairo_stroke(ctx);
}


int main(int argc, char **argv) {
  std::cout.sync_with_stdio(false);
  po::options_description general("General options");
  general.add_options()
      ("help,h", "produce this help message")
      ("version", "print the version");

  int pause_millis;
  int window_size;
  int num_particles;
  double scale_factor = 4;
  double gravity;
  double time_scale = 1.0;
  po::options_description windowopts("Display options");
  windowopts.add_options()
      ("window-size", po::value<int>(&window_size)->default_value(1000));
  windowopts.add_options()
      ("pause-millis", po::value<int>(&pause_millis)->default_value(5));
  windowopts.add_options()
      ("n,num-particles", po::value<int>(&num_particles)->default_value(3));
  windowopts.add_options()
      ("g,gravity", po::value<double>(&gravity)->default_value(1));

  po::options_description all("Allowed options");
  all.add(general).add(windowopts);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
            options(all).run(), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << all;
    return 0;
  }
  if (vm.count("version")) {
    std::cout << PACKAGE_STRING << "\n";
    return 0;
  }

  int x = window_size;
  int y = window_size;
  cairo_surface_t *sfc = cairo_create_x11_surface(&x, &y);
  cairo_t *ctx = cairo_create(sfc);

  std::vector<Particle> particles = create_world(num_particles);

  struct timespec sleep_time;
  struct timeval tv_start, tv_end, tv_diff1, tv_diff2;
  struct timeval goal { 0, pause_millis * 1000 };

  for (int running = 1; running;) {
    gettimeofday(&tv_start, nullptr);
    const double timescale = time_scale * pause_millis / 1000.0;

    move_particles(&particles, gravity, timescale);

    cairo_push_group(ctx);

    // draw the blank white background
    cairo_set_source_rgb(ctx, 1, 1, 1);
    cairo_paint(ctx);

    // draw the crosshair
    //scale(ctx, window_size, 2);
    scale(ctx, window_size, scale_factor);
    draw_crosshair(ctx);

    // draw the particles
    draw_particles(ctx, particles, scale_factor);

    cairo_pop_group_to_source(ctx);
    cairo_paint(ctx);
    cairo_surface_flush(sfc);

    switch (cairo_check_event(sfc, 0)) {
      case 0xff53:   // right cursor
        time_scale *= 1.1;
        break;

      case 0xff51:   // left cursor
        time_scale /= 1.1;
        break;

      case '-':
        scale_factor *= 1.1;
        break;

      case '+':
      case '=':
        scale_factor /= 1.1;
        break;

      case 'q':
      case 0xff1b:   // Esc
      case -1:       // left mouse button
        running = 0;
        break;
    }

    // calculate how long to sleep, and nanosleep for that long
    gettimeofday(&tv_end, nullptr);
    timersub(&tv_end, &tv_start, &tv_diff1);
    timersub(&goal, &tv_diff1, &tv_diff2);
    if (tv_diff2.tv_sec >= 0 && tv_diff2.tv_usec >= 0) {
      sleep_time.tv_sec = tv_diff2.tv_sec;
      sleep_time.tv_nsec = tv_diff2.tv_usec * 1000;
      nanosleep(&sleep_time, NULL);
    }
  }

  cairo_destroy(ctx);
  cairo_close_x11_surface(sfc);

  return 0;
}
