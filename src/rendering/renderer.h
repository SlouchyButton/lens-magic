#pragma once

#include <adwaita.h>

#include "opencl-renderer.h"

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

typedef struct RendererControl_ {
    // Image data variables
    GtkWidget*      ogl_frame;
    GdkPixbuf*      pxb_original;
    void*           image_data;
    void*           rendered_image_data;
    int             image_data_size;
    int             original_height;
    int             original_width;
    void*           preview_image_data;
    int             preview_height;
    int             preview_width;
    int             height;
    int             width;
    int             bit_depth;
    GdkTexture*     tex_rendered;

    // Adjustments
    Preset          settings;

    int             processed_fbs_count;

    // Control
    gboolean        show_original;
    char*           original_path;
    gboolean        export_pending;
    char*           export_path;

    gboolean        texture_refresh_pending;

    // Renderer instance
    CLInstance      cl_instance;
    
} RendererControl;

void redraw_image(RendererControl* con, GtkPicture* pic);