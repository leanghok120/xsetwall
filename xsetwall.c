#include <X11/X.h>
#include <X11/Xlib.h>
#include <Imlib2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Display *dpy;
Window root;

void print_help(const char *name) {
  printf("usage: %s [-c 0xRRGGBB | -p /path/to/image]\n", name);
}

void die(const char *msg) {
  fprintf(stderr, "%s\n", msg); 
  exit(1);
}

void setbgcolor(unsigned long color) {
  XSetWindowBackground(dpy, root, color);
  XClearWindow(dpy, root);
}

void setbgimage(const char *path) {
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
  image = imlib_load_image(path);
  if (image == NULL) {
    die("failed to load image");
  }
  imlib_context_set_image(image);
  imlib_render_image_on_drawable_at_size(0, 0, width, height);

  // set bg
  XSetWindowBackgroundPixmap(dpy, root, pixmap);
  XClearWindow(dpy, root);

  imlib_free_image();
  XFreePixmap(dpy, pixmap);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    print_help(argv[0]);
    return 1;
  }

  dpy = XOpenDisplay(NULL);
  root = XDefaultRootWindow(dpy);

  if (strcmp(argv[1], "-p") == 0) {
    setbgimage(argv[2]);
  } else if (strcmp(argv[1], "-c") == 0) {
    unsigned long color = strtoul(argv[2], NULL, 0);
    setbgcolor(color);
  } else {
    print_help(argv[0]);
  }

  XFlush(dpy);

  XCloseDisplay(dpy);
  return 0;
}
