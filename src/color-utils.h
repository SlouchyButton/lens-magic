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

HSLPixel rgb_to_hsl(RGBPixel rgb);
RGBPixel hsl_to_rgb(HSLPixel hsl);
