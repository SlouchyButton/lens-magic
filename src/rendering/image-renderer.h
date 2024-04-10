#pragma once

#include <adwaita.h>
#include <math.h>
#include <epoxy/gl.h>

typedef struct ShaderArgument_ {
    const gchar*      name;
    gdouble     value;
} ShaderArgument;

typedef struct ColorPreset_ {
    gdouble      color_hue;
    gdouble      color_saturation;
    gdouble      color_lightness;
} ColorPreset;

typedef struct Preset_ {
    gdouble         exposure;
    gdouble         brightness;
    gdouble         contrast;
    gdouble         highlights;
    gdouble         shadows;
    gdouble         temperature;
    gdouble         tint;
    gdouble         saturation;
    ColorPreset     color_presets[3];
    gdouble         noise_reduction;
    gdouble         noise_reduction_sharpen;
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
    GLuint      denoise;
} Programs;

typedef struct RendererControl_ {
    // Image data variables
    GtkWidget*      ogl_frame;
    GdkPixbuf*      pxb_original;
    void*           image_data;
    int             preview_height;
    int             preview_width;
    int             height;
    int             width;
    int             bit_depth;

    // Adjustments
    Preset          settings;

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
    Programs        programs;
    GLuint          VAO;
    GLuint          VBO;
    GLuint          EBO;

    int             processed_fbs_count;

    // Control
    gboolean        show_original;
    gboolean        export_pending;
    char*           export_path;

    gboolean        texture_refresh_pending;
    
} RendererControl;

void refresh_textures(RendererControl* con);
gboolean render (GtkGLArea* area, GdkGLContext* context, RendererControl* con);
void unrealize (GtkWidget *widget);
void realize (GtkWidget *widget, RendererControl* con);
gboolean export(RendererControl* con, char* path);
void redraw_image(GtkGLArea* gl_area);
