#pragma once

#include <adwaita.h>
#include <math.h>
#include <epoxy/gl.h>

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

typedef struct Programs_ {
    GLuint      plain;
    GLuint      exposure;
    GLuint      temperature;
} Programs;

typedef struct RendererControl_ {
    // Image data variables
    GtkWidget       *ogl_frame;
    GdkPixbuf       *pxb_original;
    GdkTexture      *tex_rendered;
    // Control variables
    GMutex          data_mutex;
    GCond           data_cond;
    gboolean        pending_refresh;
    gboolean        pending_new_picture;
    gboolean        kill_thread;
    // Adjustments
    Preset          settings;
    // OpenGL variables
    GLuint          fb1;
    GLuint          fb2;
    GLuint          tex_base;
    GLuint          tex_fb1;
    GLuint          tex_fb2;
    Programs        programs;
    GLuint          VAO;
    GLuint          VBO;
    GLuint          EBO;
} RendererControl;

gboolean render (GtkGLArea* area, GdkGLContext* context, RendererControl* con);
void unrealize (GtkWidget *widget);
void realize (GtkWidget *widget, RendererControl* con);