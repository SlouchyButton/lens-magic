#pragma once

#include <adwaita.h>
#include <math.h>

typedef struct RGBPixel_ {
  guchar r;
  guchar g;
  guchar b;
} RGBPixel;

// h - hue in degrees
// s - saturation from 0 to 1 (1 being 100 %)
// l
typedef struct HSLPixel_ {
  gdouble h;
  gdouble s;
  gdouble l;
} HSLPixel;

typedef struct PixelsRGB_ {
    RGBPixel* pixels;
    gulong size;
    guint rowstride;
}

typedef struct PixelsHSL_ {
    HSLPixel* pixels;
    gulong size;
    guint rowstride;
}

// caller takes ownership of pixels and is responsible of freeing them
HSLPixel get_neighbors_hsl(GdkPixbuf* pxb, guint x, guint y, guint size);
RGBPixel hsl_to_rgb(HSLPixel hsl);
