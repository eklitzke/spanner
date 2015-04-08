#include <sys/time.h>
#include <time.h>

#include <boost/program_options.hpp>

#include <iostream>
#include <random>
#include <vector>

#include "../config.h"
#include "./particle.h"
#include "./x11.h"

namespace po = boost::program_options;



int main(int argc, char **argv) {
  std::cout.sync_with_stdio(false);
  po::options_description general("General options");
  general.add_options()
      ("help,h", "produce this help message")
      ("version", "print the version");

  int pause_millis;
  int window_size;
  int num_particles;
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
  windowopts.add_options()
      ("sun-mass", po::value<double>()->default_value(10));

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

  cairo_surface_t *sfc;
  cairo_t *ctx;
  int x, y;

  int running;

  x = y = window_size;
  sfc = cairo_create_x11_surface(&x, &y);
  ctx = cairo_create(sfc);

  std::vector<Particle> particles;
  {
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<double> dist(-1, 1);

    Particle p{{0, 0}, {0, 0}, vm["sun-mass"].as<double>()};
    particles.push_back(p);
    for (int i = 0; i < num_particles; i++) {
      Particle p{
	{3 * dist(engine), 3 * dist(engine)},
	{3 * dist(engine), 3 * dist(engine)},
	    1};
      particles.push_back(p);
    }
  }

  cairo_scale(ctx, window_size / 8, window_size / 8);
  cairo_translate(ctx, 4, 4);

#if 0
  double cx = 0;
  double cy = 0;
  cairo_user_to_device(ctx, &cx, &cy);
  std::cout << "center is " << cx << " " << cy << std::endl;
#endif

  struct timespec sleep_time;
  struct timeval tv_start, tv_end, tv_diff1, tv_diff2;
  struct timeval goal { 0, pause_millis * 1000 };

  for (running = 1; running;) {
    gettimeofday(&tv_start, nullptr);
    const double timescale = time_scale * pause_millis / 1000.0;

    for (Particle &p : particles) {
      p.zero_force();
      if (p.mass() == 1) {
	for (const Particle &o : particles) {
	  p.augment_force(gravity, o);
	}
      }
    }
    for (Particle &p : particles) {
      p.augment_velocity(timescale);
      p.augment_position(timescale);
      double x = p.position().x();
      double y = p.position().y();
      cairo_user_to_device(ctx, &x, &y);
    }

    cairo_push_group(ctx);
    cairo_set_source_rgb(ctx, 1, 1, 1);
    cairo_paint(ctx);

    cairo_set_source_rgb(ctx, 0, 0, 0);
    for (const Particle &p : particles) {
#if 0
      double px = p.position().x();
      double py = p.position().y();
      cairo_user_to_device(ctx, &px, &py);
      std::cout << "position = " << p.position() << ", "
		<< "velocity = " << p.velocity() << ", "
		<< "device coordinates are "
		<< "(" << px << ", " << py << ")\n";
#endif

      cairo_arc(ctx,
		p.position().x(),
		p.position().y(),
		sqrt(p.mass()) * 0.03,
		0,
		2 * M_PI);

      cairo_set_line_width(ctx, 0.01);
      cairo_stroke(ctx);
    }

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
