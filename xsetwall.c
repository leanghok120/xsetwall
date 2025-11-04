#include <X11/X.h>
#include <X11/Xlib.h>
#include <Imlib2.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("usage: %s path\n", argv[0]);
    return 1;
  }

  // setup xlib
  Display *dpy = XOpenDisplay(NULL);
  Window root = XDefaultRootWindow(dpy);
  int scr = DefaultScreen(dpy);
  int width = DisplayWidth(dpy, scr);
  int height = DisplayHeight(dpy, scr);
  Pixmap pixmap = XCreatePixmap(dpy, root, width, height, DefaultDepth(dpy, scr));

  // setup imlib
  imlib_context_set_display(dpy);
  imlib_context_set_visual(DefaultVisual(dpy, scr));
  imlib_context_set_colormap(DefaultColormap(dpy, scr));
  imlib_context_set_drawable(pixmap);

  // load image
  Imlib_Image image;
  image = imlib_load_image(argv[1]);
  if (image == NULL) {
    fprintf(stderr, "failed to load image\n"); 
    exit(1);
  }
  imlib_context_set_image(image);
  imlib_render_image_on_drawable_at_size(0, 0, width, height);

  // set bg
  XSetWindowBackgroundPixmap(dpy, root, pixmap);
  XClearWindow(dpy, root);
  XFlush(dpy);

  imlib_free_image();
  XFreePixmap(dpy, pixmap);
  XCloseDisplay(dpy);
  return 0;
}
