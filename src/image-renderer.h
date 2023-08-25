#pragma once

#include <adwaita.h>
#include <math.h>

typedef struct Preset_ {
    gdouble      exposure;
    gdouble      brightness;
    gdouble      contrast;
    gdouble      highlights;
    gdouble      shadows;
    gdouble      temperature;
    gdouble      tint;
    gdouble      saturation;
    gdouble      color_hue;
    gdouble      color_saturation;
    gdouble      color_lightness;
} Preset;

typedef struct RendererControl_ {
    GtkPicture      *picture;
    GdkPixbuf       *pxb_original;
    GdkTexture      *tex_rendered;
    GMutex          data_mutex;
    GCond           data_cond;
    gboolean        pending_refresh;
    Preset          settings;
    gboolean        kill_thread;
} RendererControl;

void render_pixbuf(GdkPixbuf* pxb, Preset settings);
gpointer renderer(gpointer data);

