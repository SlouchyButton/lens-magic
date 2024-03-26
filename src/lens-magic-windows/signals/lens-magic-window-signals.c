#include "lens-magic-window-signals.h"

void set_entry_value(GtkEntry* entry, gdouble value) {
    GString* str_val = g_string_new(NULL);
    if (value == 100 || value == -100) {
        g_string_printf(str_val, "%.0f", value);
    } else {
        g_string_printf(str_val, "%.2f", value);
    }
    GtkEntryBuffer* entry_buf = gtk_entry_get_buffer(entry);
    gtk_entry_buffer_set_text (entry_buf, str_val->str, -1);
}

// Light callbacks

void exposure_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->exposure_entry, val);
    self->con.settings.exposure = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void brightness_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->brightness_entry, val);
    self->con.settings.brightness = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void contrast_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->contrast_entry, val);
    self->con.settings.contrast = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void highlights_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->highlights_entry, val);
    self->con.settings.highlights = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void shadows_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->shadows_entry, val);
    self->con.settings.shadows = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

// Color callbacks

void temperature_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->temperature_entry, val);
    self->con.settings.temperature = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void tint_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->tint_entry, val);
    self->con.settings.tint = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void saturation_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->saturation_entry, val);
    self->con.settings.saturation = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}


// Color filter callbacks
void filter_red_button_clicked(GtkToggleButton* btn, LensMagicWindow* self) {
    return;
}
void filter_green_button_clicked(GtkToggleButton* btn, LensMagicWindow* self) {
    return;
}
void filter_blue_button_clicked(GtkToggleButton* btn, LensMagicWindow* self) {
    return;
}

void color_hue_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->color_hue_entry, val);

    if (gtk_toggle_button_get_active(self->filter_red_button)) {
        self->con.settings.color_presets[0].color_hue = val;
    }
    if (gtk_toggle_button_get_active(self->filter_green_button)) {
        self->con.settings.color_presets[1].color_hue = val;
    }
    if (gtk_toggle_button_get_active(self->filter_blue_button)) {
        self->con.settings.color_presets[2].color_hue = val;
    }

    redraw_image ((GtkGLArea*)self->gl_area);
}

void color_saturation_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->color_saturation_entry, val);

    if (gtk_toggle_button_get_active(self->filter_red_button)) {
        self->con.settings.color_presets[0].color_saturation = val;
    }
    if (gtk_toggle_button_get_active(self->filter_green_button)) {
        self->con.settings.color_presets[1].color_saturation = val;
    }
    if (gtk_toggle_button_get_active(self->filter_blue_button)) {
        self->con.settings.color_presets[2].color_saturation = val;
    }

    redraw_image ((GtkGLArea*)self->gl_area);
}

void color_lightness_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->color_lightness_entry, val);
    
    if (gtk_toggle_button_get_active(self->filter_red_button)) {
        self->con.settings.color_presets[0].color_lightness = val;
    }
    if (gtk_toggle_button_get_active(self->filter_green_button)) {
        self->con.settings.color_presets[1].color_lightness = val;
    }
    if (gtk_toggle_button_get_active(self->filter_blue_button)) {
        self->con.settings.color_presets[2].color_lightness = val;
    }

    redraw_image ((GtkGLArea*)self->gl_area);
}

