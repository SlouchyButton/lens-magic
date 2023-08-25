#include "color-utils.h"

HSLPixel rgb_to_hsl(RGBPixel rgb) {
    HSLPixel out;

    gdouble r = rgb.r / 255.0;
    gdouble g = rgb.g / 255.0;
    gdouble b = rgb.b / 255.0;

    gdouble max_val = MAX(r, MAX(g, b));
    gdouble min_val = MIN(r, MIN(g, b));
    gdouble delta = max_val - min_val;

    if (delta == 0) {
        out.h = 0;
        out.s = 0;
        out.l = r;
        return out;
    }

    if (max_val == r) {
        out.h = 60 * fmod(((g-b)/delta), 6);
    }
    if (max_val == g) {
        out.h = 60 * ((b-r)/delta + 2);
    }
    if (max_val == b) {
        out.h = 60 * ((r-g)/delta + 4);
    }

    out.l = (max_val + min_val) / 2;
    out.s = delta / (1 - fabs(2*out.l - 1));

    return out;
}

RGBPixel hsl_to_rgb(HSLPixel hsl) {
    RGBPixel out;

    gdouble c = hsl.s * (1 - fabs(2*hsl.l-1));
    gdouble x = c * (1 - fabs(fmod(hsl.h/60, 2) - 1));
    gdouble m = hsl.l - c/2;

    gdouble r_temp = 0;
    gdouble g_temp = 0;
    gdouble b_temp = 0;

    if (hsl.h >= 0 && hsl.h < 60) {
        r_temp = c;
        g_temp = x;
    }
    if (hsl.h >= 60 && hsl.h < 120) {
        r_temp = x;
        g_temp = c;
    }
    if (hsl.h >= 120 && hsl.h < 180) {
        g_temp = c;
        b_temp = x;
    }
    if (hsl.h >= 180 && hsl.h < 240) {
        g_temp = x;
        b_temp = c;
    }
    if (hsl.h >= 240 && hsl.h < 300) {
        r_temp = x;
        b_temp = c;
    }
    if (hsl.h >= 300 && hsl.h < 360) {
        r_temp = c;
        b_temp = x;
    }

    out.r = (r_temp+m)*255;
    out.g = (g_temp+m)*255;
    out.b = (b_temp+m)*255;

    return out;
}
