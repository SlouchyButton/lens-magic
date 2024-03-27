
#pragma once

#include <adwaita.h>
#include <math.h>
#include "rendering/image-renderer.h"

G_BEGIN_DECLS

#define LENS_MAGIC_TYPE_WINDOW (lens_magic_window_get_type())

G_DECLARE_FINAL_TYPE (LensMagicWindow, lens_magic_window, LENS_MAGIC, WINDOW, AdwApplicationWindow)

G_END_DECLS

struct _LensMagicWindow {
    AdwApplicationWindow parent_instance;

    RendererControl con;

    /* Template widgets */
    GtkHeaderBar*       header_bar;
    GtkButton*          open_file_button;
    GtkButton*          export_button;

    GtkWidget*          testbox;
    GtkWidget*          gl_area;
    GdkPixbuf*          pxb_original;

    GtkToggleButton*    filter_red_button;
    GtkToggleButton*    filter_green_button;
    GtkToggleButton*    filter_blue_button;
    int                 selected_filter;

    GtkScale*           exposure_scale;
    GtkScale*           brightness_scale;
    GtkScale*           contrast_scale;
    GtkScale*           highlights_scale;
    GtkScale*           shadows_scale;
    GtkScale*           temperature_scale;
    GtkScale*           tint_scale;
    GtkScale*           saturation_scale;
    GtkScale*           color_hue_scale;
    GtkScale*           color_saturation_scale;
    GtkScale*           color_lightness_scale;

    GtkEntry*           exposure_entry;
    GtkEntry*           brightness_entry;
    GtkEntry*           contrast_entry;
    GtkEntry*           highlights_entry;
    GtkEntry*           shadows_entry;
    GtkEntry*           temperature_entry;
    GtkEntry*           tint_entry;
    GtkEntry*           saturation_entry;
    GtkEntry*           color_hue_entry;
    GtkEntry*           color_saturation_entry;
    GtkEntry*           color_lightness_entry;
};

