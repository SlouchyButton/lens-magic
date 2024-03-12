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
    GLuint      brightness;
    GLuint      contrast;
    GLuint      highlights;
    GLuint      shadows;
    GLuint      temperature;
    GLuint      tint;
    GLuint      saturation;
    GLuint      color_hue;
    GLuint      color_saturation;
    GLuint      color_lightness;
} Programs;

typedef struct RendererControl_ {
    // Image data variables
    GtkWidget*      ogl_frame;
    GdkPixbuf*      pxb_original;
    unsigned short* image_data;
    int             height;
    int             width;

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

void refresh_textures(RendererControl* con);
void refresh_textures_raw(RendererControl* con);
gboolean render (GtkGLArea* area, GdkGLContext* context, RendererControl* con);
void unrealize (GtkWidget *widget);
void realize (GtkWidget *widget, RendererControl* con);
gboolean export(RendererControl* con, char* path);
void redraw_image(GtkGLArea* gl_area);
