#include "./x11.h"

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <cairo/cairo-xlib.h>

#include <cassert>
#include <iostream>

int cairo_check_event(cairo_surface_t *sfc, int block) {
  char keybuf[8];
  KeySym key;
  XEvent e;

  for (;;) {
    if (block || XPending(cairo_xlib_surface_get_display(sfc))) {
      XNextEvent(cairo_xlib_surface_get_display(sfc), &e);
    } else {
      return 0;
    }

    switch (e.type) {
      case ButtonPress:
        return -e.xbutton.button;
      case KeyPress:
        XLookupString(&e.xkey, keybuf, sizeof(keybuf), &key, NULL);
        return key;
      default:
        std::cerr << "Dropping unhandled XEevent.type = " << e.type << ".\n";
    }
  }
}


/*! Open an X11 window and create a cairo surface base on that window. If x and
 * y are set to 0 the function opens a full screen window and stores to window
 * dimensions to x and y.
 * @param x Pointer to width of window.
 * @param y Pointer to height of window.
 * @return Returns a pointer to a valid Xlib cairo surface. The function does
 * not return on error (exit(3)).
 */
cairo_surface_t *cairo_create_x11_surface(int *x, int *y) {
  Display *dsp;
  Drawable da;
  Screen *scr;
  int screen;
  cairo_surface_t *sfc;

  if ((dsp = XOpenDisplay(NULL)) == NULL)
    exit(1);
  screen = DefaultScreen(dsp);
  scr = DefaultScreenOfDisplay(dsp);
  if (!*x || !*y) {
    assert(false);
    *x = WidthOfScreen(scr), *y = HeightOfScreen(scr);
    da = XCreateSimpleWindow(dsp, DefaultRootWindow(dsp), 0, 0, *x, *y, 0, 0, 0);
#if 0
    fullscreen (dsp, da);
#endif
  } else {
    da = XCreateSimpleWindow(dsp, DefaultRootWindow(dsp), 0, 0, *x, *y, 0, 0, 0);
  }
  XStoreName(dsp, da, "n-Body Simulator");
  XSelectInput(dsp, da, ButtonPressMask | KeyPressMask);
  XMapWindow(dsp, da);

  sfc = cairo_xlib_surface_create(dsp, da, DefaultVisual(dsp, screen), *x, *y);
  cairo_xlib_surface_set_size(sfc, *x, *y);

  return sfc;
}


/*! Destroy cairo Xlib surface and close X connection.
 */
void cairo_close_x11_surface(cairo_surface_t *sfc) {
  Display *dsp = cairo_xlib_surface_get_display(sfc);

  cairo_surface_destroy(sfc);
  XCloseDisplay(dsp);
}
