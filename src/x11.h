// -*- C++ -*-

#ifndef SPANNER_X11_H_
#define SPANNER_X11_H_

#include <cairo/cairo.h>

int cairo_check_event(cairo_surface_t *sfc, int block);

cairo_surface_t *cairo_create_x11_surface(int *x, int *y);

void cairo_close_x11_surface(cairo_surface_t *sfc);

#endif  // SPANNER_X11_H_
