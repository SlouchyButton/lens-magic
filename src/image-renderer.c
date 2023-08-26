#include "image-renderer.h"
#include "color-utils.h"
#include "hw-renderer.h"

void exposure_apply(GdkPixbuf* pxb, gdouble val);
void brightness_apply(GdkPixbuf* pxb, gdouble val);
void contrast_apply(GdkPixbuf* pxb, gdouble val);
void highlights_apply(GdkPixbuf* pxb, gdouble val);
void shadows_apply(GdkPixbuf* pxb, gdouble val);
void temperature_apply(GdkPixbuf* pxb, gdouble val);
void tint_apply(GdkPixbuf* pxb, gdouble val);
void saturation_apply(GdkPixbuf* pxb, gdouble val);
void color_hue_apply(GdkPixbuf* pxb, gdouble val);
void color_saturation_apply(GdkPixbuf* pxb, gdouble val);
void color_lightness_apply(GdkPixbuf* pxb, gdouble val);

void exposure_apply(GdkPixbuf* pxb, gdouble val) {
    gboolean has_alpha = gdk_pixbuf_get_has_alpha (pxb);

    guint len = 0;
    guchar* pix = NULL;
    pix = gdk_pixbuf_get_pixels_with_length (pxb, &len);
    /*for (int i = 0; i < len; i = i+3+has_alpha) {
      for (int j = 0; j < 3; j++) {
        int new = pix[i+j] * pow(2, val);
        if (new > 255) {
          pix[i] = 255;
          pix[i+1] = 255;
          pix[i+2] = 255;
          break;
        } else if (new < 0) {
          pix[i] = 0;
          pix[i+1] = 0;
          pix[i+2] = 0;
          break;
        } else {
          pix[i+j] = new;
        }
      }
    }*/

    hw_set("exposure", pix, 3+has_alpha, len, val);
}

void brightness_apply(GdkPixbuf* pxb, gdouble val) {
    gboolean has_alpha = gdk_pixbuf_get_has_alpha (pxb);

    guint len = 0;
    guchar* pix = NULL;
    pix = gdk_pixbuf_get_pixels_with_length (pxb, &len);
    /*for (int i = 0; i < len; i = i+3+has_alpha) {
      for (int j = 0; j < 3; j++) {
        int new = pix[i+j] + val;
        if (new > 255) {
          pix[i] = 255;
          pix[i+1] = 255;
          pix[i+2] = 255;
          break;
        } else if (new < 0) {
          pix[i] = 0;
          pix[i+1] = 0;
          pix[i+2] = 0;
          break;
        } else {
          pix[i+j] = new;
        }
      }
    }*/

    hw_set("brightness", pix, 3+has_alpha, len, val);
}

void contrast_apply(GdkPixbuf* pxb, gdouble val) {
    gboolean has_alpha = gdk_pixbuf_get_has_alpha (pxb);

    guint len = 0;
    guchar* pix = NULL;
    pix = gdk_pixbuf_get_pixels_with_length (pxb, &len);
    /*for (int i = 0; i < len; i = i+3+has_alpha) {
      for (int j = 0; j < 3; j++) {
        int new = val*(pix[i+j]-128)+128;
        if (new > 255) {
          pix[i] = 255;
          pix[i+1] = 255;
          pix[i+2] = 255;
          break;
        } else if (new < 0) {
          pix[i] = 0;
          pix[i+1] = 0;
          pix[i+2] = 0;
          break;
        } else {
          pix[i+j] = new;
        }
      }
    }*/

    hw_set("contrast", pix, 3+has_alpha, len, val);
}

void temperature_apply(GdkPixbuf* pxb, gdouble val) {
    gboolean has_alpha = gdk_pixbuf_get_has_alpha (pxb);

    guint len = 0;
    guchar* pix = NULL;
    pix = gdk_pixbuf_get_pixels_with_length (pxb, &len);
    /*for (int i = 0; i < len; i = i+3+has_alpha) {
        int newR = pix[i] + val;
        if (newR > 255) {
          pix[i] = 255;
        } else if (newR < 0) {
          pix[i] = 0;
        } else {
          pix[i] = newR;
        }
        int newB = pix[i+2] - val;
        if (newB > 255) {
          pix[i+2] = 255;
        } else if (newB < 0) {
          pix[i+2] = 0;
        } else {
          pix[i+2] = newB;
        }
    }*/

    hw_set("temperature", pix, 3+has_alpha, len, val);
}

void tint_apply(GdkPixbuf* pxb, gdouble val) {
    gboolean has_alpha = gdk_pixbuf_get_has_alpha (pxb);

    guint len = 0;
    guchar* pix = NULL;
    pix = gdk_pixbuf_get_pixels_with_length (pxb, &len);
    for (int i = 0; i < len; i = i+3+has_alpha) {
        int newR = pix[i] + val;
        if (newR > 255) {
          pix[i] = 255;
        } else if (newR < 0) {
          pix[i] = 0;
        } else {
          pix[i] = newR;
        }
        int newB = pix[i+2] + val;
        if (newB > 255) {
          pix[i+2] = 255;
        } else if (newB < 0) {
          pix[i+2] = 0;
        } else {
          pix[i+2] = newB;
        }
        int newG = pix[i+1] - val;
        if (newG > 255) {
          pix[i+1] = 255;
        } else if (newG < 0) {
          pix[i+1] = 0;
        } else {
          pix[i+1] = newG;
        }
    }
}

void saturation_apply(GdkPixbuf* pxb, gdouble val) {
    gboolean has_alpha = gdk_pixbuf_get_has_alpha (pxb);

    guint len = 0;
    guchar* pix = NULL;
    pix = gdk_pixbuf_get_pixels_with_length (pxb, &len);
    /*for (int i = 0; i < len; i = i+3+has_alpha) {
        RGBPixel rgb = {pix[i], pix[i+1], pix[i+2]};

        HSLPixel hsl = rgb_to_hsl(rgb);

        gdouble modifier = hsl.s * val;

        if (hsl.s + modifier > 1) {
            hsl.s = 1;
        } else if (hsl.s + modifier < 0) {
            hsl.s = 0;
        } else {
            hsl.s += modifier;
        }

        rgb = hsl_to_rgb(hsl);
        pix[i] = rgb.r;
        pix[i+1] = rgb.g;
        pix[i+2] = rgb.b;
    }*/
    hw_set("saturation", pix, 3+has_alpha, len, val);
}

void highlights_apply(GdkPixbuf* pxb, gdouble val) {
    gboolean has_alpha = gdk_pixbuf_get_has_alpha (pxb);

    guint len = 0;
    guchar* pix = NULL;
    pix = gdk_pixbuf_get_pixels_with_length (pxb, &len);
    for (int i = 0; i < len; i = i+3+has_alpha) {
        RGBPixel rgb = {pix[i], pix[i+1], pix[i+2]};

        HSLPixel hsl = rgb_to_hsl(rgb);

        gdouble coefficient = (1 - hsl.l*2.5)*-1;
        if (coefficient < 0)
            coefficient = 0;

        /*if (hsl.l + val*coefficient > 1) {
            hsl.l = 1;
        } else if (hsl.l + val*coefficient < 0) {
            hsl.l = 0;
        } else {
            hsl.l += val*coefficient;
        }

        rgb = hsl_to_rgb(hsl);
        pix[i] = rgb.r;
        pix[i+1] = rgb.g;
        pix[i+2] = rgb.b;*/

        gdouble modifier = (val) * coefficient + 1;

        for (int j = 0; j < 3; j++) {
            int new = modifier*(pix[i+j]-128)+128;
            if (new > 255) {
              pix[i] = 255;
              pix[i+1] = 255;
              pix[i+2] = 255;
              break;
            } else if (new < 0) {
              pix[i] = 0;
              pix[i+1] = 0;
              pix[i+2] = 0;
              break;
            } else {
              pix[i+j] = new;
            }
        }
    }
}

void shadows_apply(GdkPixbuf* pxb, gdouble val) {
    gboolean has_alpha = gdk_pixbuf_get_has_alpha (pxb);

    guint len = 0;
    guchar* pix = NULL;
    pix = gdk_pixbuf_get_pixels_with_length (pxb, &len);
    for (int i = 0; i < len; i = i+3+has_alpha) {
        RGBPixel rgb = {pix[i], pix[i+1], pix[i+2]};

        HSLPixel hsl = rgb_to_hsl(rgb);

        gdouble coefficient = 1 - hsl.l*2.5;
        if (coefficient < 0)
            coefficient = 0;

        /*if (hsl.l + val*coefficient > 1) {
            hsl.l = 1;
        } else if (hsl.l + val*coefficient < 0) {
            hsl.l = 0;
        } else {
            hsl.l += val*coefficient;
        }

        rgb = hsl_to_rgb(hsl);
        pix[i] = rgb.r;
        pix[i+1] = rgb.g;
        pix[i+2] = rgb.b;*/

        gdouble modifier = (val * -1) * coefficient + 1;

        for (int j = 0; j < 3; j++) {
            int new = modifier*(pix[i+j]-128)+128;
            if (new > 255) {
              pix[i] = 255;
              pix[i+1] = 255;
              pix[i+2] = 255;
              break;
            } else if (new < 0) {
              pix[i] = 0;
              pix[i+1] = 0;
              pix[i+2] = 0;
              break;
            } else {
              pix[i+j] = new;
            }
        }
    }
}

void color_hue_apply(GdkPixbuf* pxb, gdouble val) {
    gboolean has_alpha = gdk_pixbuf_get_has_alpha (pxb);

    guint len = 0;
    guchar* pix = NULL;
    pix = gdk_pixbuf_get_pixels_with_length (pxb, &len);
    for (int i = 0; i < len; i = i+3+has_alpha) {
        RGBPixel rgb = {pix[i], pix[i+1], pix[i+2]};

        HSLPixel hsl = rgb_to_hsl(rgb);

        if (hsl.h > 30 && hsl.h < 330)
            continue;

        if (hsl.h - val < 0) {
            hsl.h = 360 - val + hsl.h;
        } else {
            hsl.h = hsl.h - val;
        }

        rgb = hsl_to_rgb(hsl);
        pix[i] = rgb.r;
        pix[i+1] = rgb.g;
        pix[i+2] = rgb.b;
    }
}

void color_saturation_apply(GdkPixbuf* pxb, gdouble val) {
    gboolean has_alpha = gdk_pixbuf_get_has_alpha (pxb);

    guint len = 0;
    guchar* pix = NULL;
    pix = gdk_pixbuf_get_pixels_with_length (pxb, &len);
    for (int i = 0; i < len; i = i+3+has_alpha) {
        RGBPixel rgb = {pix[i], pix[i+1], pix[i+2]};

        HSLPixel hsl = rgb_to_hsl(rgb);

        if (hsl.h > 30 && hsl.h < 330)
            continue;

        gdouble modifier = hsl.s * val;

        if (hsl.s + modifier > 1) {
            hsl.s = 1;
        } else if (hsl.s + modifier < 0) {
            hsl.s = 0;
        } else {
            hsl.s += modifier;
        }

        rgb = hsl_to_rgb(hsl);
        pix[i] = rgb.r;
        pix[i+1] = rgb.g;
        pix[i+2] = rgb.b;
    }
}

void color_lightness_apply(GdkPixbuf* pxb, gdouble val) {
    gboolean has_alpha = gdk_pixbuf_get_has_alpha (pxb);

    guint len = 0;
    guchar* pix = NULL;
    pix = gdk_pixbuf_get_pixels_with_length (pxb, &len);
    for (int i = 0; i < len; i = i+3+has_alpha) {
        RGBPixel rgb = {pix[i], pix[i+1], pix[i+2]};

        HSLPixel hsl = rgb_to_hsl(rgb);

        if (hsl.h > 30 && hsl.h < 330)
            continue;

        gdouble modifier = hsl.l * val;

        if (hsl.l + modifier > 1) {
            hsl.l = 1;
        } else if (hsl.l + modifier < 0) {
            hsl.l = 0;
        } else {
            hsl.l += modifier;
        }

        rgb = hsl_to_rgb(hsl);
        pix[i] = rgb.r;
        pix[i+1] = rgb.g;
        pix[i+2] = rgb.b;
    }
}

void render_pixbuf(GdkPixbuf* pxb, Preset settings) {
    if (settings.exposure != 0)
        exposure_apply (pxb, settings.exposure);
    if (settings.brightness != 0)
        brightness_apply (pxb, settings.brightness);
    if (settings.contrast != 1 && settings.contrast != 0)
        contrast_apply (pxb, settings.contrast);
    if (settings.highlights != 0)
        highlights_apply (pxb, settings.highlights);
    if (settings.shadows != 0)
        shadows_apply (pxb, settings.shadows);
    if (settings.saturation != 0)
        saturation_apply (pxb, settings.saturation);
    if (settings.temperature != 0)
        temperature_apply (pxb, settings.temperature);
    if (settings.tint != 0)
        tint_apply (pxb, settings.tint);
    if (settings.color_hue != 0)
        color_hue_apply (pxb, settings.color_hue);
    if (settings.color_saturation != 0)
        color_saturation_apply (pxb, settings.color_saturation);
    if (settings.color_lightness != 0)
        color_lightness_apply (pxb, settings.color_lightness);
}

gboolean set_picture(gpointer data) {
    RendererControl* con = (RendererControl*)data;

    gtk_picture_set_paintable (con->picture, GDK_PAINTABLE(con->tex_rendered));

    return false;
}

gpointer renderer(gpointer data) {
    RendererControl* con = (RendererControl*)data;

    hw_init();

    while (!con->kill_thread) {
        g_mutex_lock (&con->data_mutex);
        while (!con->pending_refresh)
            g_cond_wait (&con->data_cond, &con->data_mutex);
        Preset settings = con->settings;
        con->pending_refresh = false;
        g_mutex_unlock (&con->data_mutex);

        GdkPixbuf* pxb = gdk_pixbuf_copy(con->pxb_original);

        render_pixbuf (pxb, settings);

        g_object_unref (con->tex_rendered);
        con->tex_rendered = gdk_texture_new_for_pixbuf(pxb);
        g_object_unref (pxb);

        g_idle_add (set_picture, data);
    }

    return NULL;
}
