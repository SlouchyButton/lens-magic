#pragma once

#include <adwaita.h>
#include <math.h>
#include <epoxy/gl.h>

#include "renderer.h"

typedef struct GLPrograms_ {
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
    GLuint      denoise;
} GLPrograms;

typedef struct GLInstance_ {
    // OpenGL variables
    GLuint          fb1;
    GLuint          fb2;
    GLuint          tex_base;
    GLuint          tex_fb1;
    GLuint          tex_fb2;
    GLuint          preview_fb1;
    GLuint          preview_fb2;
    GLuint          preview_tex_fb1;
    GLuint          preview_tex_fb2;
    GLPrograms      programs;
    GLuint          VAO;
    GLuint          VBO;
    GLuint          EBO;
    GLint           max_tex_size;
} GLInstance;

typedef struct ShaderArgument_ {
    const gchar*      name;
    gdouble     value;
} ShaderArgument;

void refresh_textures(RendererControl* con);
gboolean render (GtkGLArea* area, GdkGLContext* context, RendererControl* con);
void unrealize (GtkWidget *widget);
void realize (GtkWidget *widget, RendererControl* con);
gboolean export(RendererControl* con, char* path);
//void redraw_image(GtkGLArea* gl_area);
