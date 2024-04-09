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

//----------------//
// Misc callbacks //
//----------------//
gboolean original_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow* self) {
    self->con.show_original = state;
    redraw_image ((GtkGLArea*)self->gl_area);
    return false;
}

gboolean adj_switch_state_set(GtkSwitch* sw, gboolean state, AdjustmentElements* data) {
    if (state) {
        gdouble val = gtk_range_get_value ((GtkRange*) data->scale);
        *data->settings_value = val;
    } else {
        *data->settings_value = 0;
    }

    redraw_image ((GtkGLArea*)data->self->gl_area);
    return false;
}

void adj_scale_change(GtkRange* range, AdjustmentElements* data) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(data->entry, val);
    *data->settings_value = val;

    gtk_switch_set_active(data->sw, true);

    redraw_image ((GtkGLArea*)data->self->gl_area);
}

//-----------------//
// Light callbacks //
//-----------------//

/*gboolean exposure_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow *self) {
    if (state) {
        gdouble val = gtk_range_get_value ((GtkRange*) self->exposure_scale);
        self->con.settings.exposure = val;
    } else {
        self->con.settings.exposure = 0;
    }

    redraw_image ((GtkGLArea*)self->gl_area);
    return false;
}

gboolean brightness_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow *self) {
    if (state) {
        gdouble val = gtk_range_get_value ((GtkRange*) self->brightness_scale);
        self->con.settings.brightness = val;
    } else {
        self->con.settings.brightness = 0;
    }

    redraw_image ((GtkGLArea*)self->gl_area);
    return false;
}

gboolean contrast_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow *self) {
    if (state) {
        gdouble val = gtk_range_get_value ((GtkRange*) self->contrast_scale);
        self->con.settings.contrast = val;
    } else {
        self->con.settings.contrast = 0;
    }

    redraw_image ((GtkGLArea*)self->gl_area);
    return false;
}

gboolean highlights_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow *self) {
    if (state) {
        gdouble val = gtk_range_get_value ((GtkRange*) self->highlights_scale);
        self->con.settings.highlights = val;
    } else {
        self->con.settings.highlights = 0;
    }

    redraw_image ((GtkGLArea*)self->gl_area);
    return false;
}

gboolean shadows_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow *self) {
    if (state) {
        gdouble val = gtk_range_get_value ((GtkRange*) self->shadows_scale);
        self->con.settings.shadows = val;
    } else {
        self->con.settings.shadows = 0;
    }

    redraw_image ((GtkGLArea*)self->gl_area);
    return false;
}

void exposure_scale_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->exposure_entry, val);
    self->con.settings.exposure = val;

    gtk_switch_set_active(self->exposure_switch, true);

    redraw_image ((GtkGLArea*)self->gl_area);
}

void brightness_scale_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->brightness_entry, val);
    self->con.settings.brightness = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void contrast_scale_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->contrast_entry, val);
    self->con.settings.contrast = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void highlights_scale_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->highlights_entry, val);
    self->con.settings.highlights = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void shadows_scale_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->shadows_entry, val);
    self->con.settings.shadows = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}


//-----------------//
// Color callbacks //
//-----------------//

void temperature_scale_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->temperature_entry, val);
    self->con.settings.temperature = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void tint_scale_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->tint_entry, val);
    self->con.settings.tint = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void saturation_scale_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->saturation_entry, val);
    self->con.settings.saturation = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}*/


//------------------------//
// Color filter callbacks //
//------------------------//

void refresh_ranges(LensMagicWindow* self) {
    gtk_range_set_value((GtkRange*)self->color_hue_scale, 
        self->con.settings.color_presets[self->selected_filter].color_hue);
    gtk_range_set_value((GtkRange*)self->color_saturation_scale, 
        self->con.settings.color_presets[self->selected_filter].color_saturation);
    gtk_range_set_value((GtkRange*)self->color_lightness_scale, 
        self->con.settings.color_presets[self->selected_filter].color_lightness);

    set_entry_value(self->color_hue_entry, 
        self->con.settings.color_presets[self->selected_filter].color_hue);
    set_entry_value(self->color_saturation_entry, 
        self->con.settings.color_presets[self->selected_filter].color_saturation);
    set_entry_value(self->color_lightness_entry, 
        self->con.settings.color_presets[self->selected_filter].color_lightness);
}

void filter_red_button_clicked(GtkToggleButton* btn, LensMagicWindow* self) {
    self->selected_filter = 0;
    refresh_ranges(self);
}

void filter_green_button_clicked(GtkToggleButton* btn, LensMagicWindow* self) {
    self->selected_filter = 1;
    refresh_ranges(self);
}

void filter_blue_button_clicked(GtkToggleButton* btn, LensMagicWindow* self) {
    self->selected_filter = 2;
    refresh_ranges(self);
}

void color_hue_scale_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->color_hue_entry, val);

    self->con.settings.color_presets[self->selected_filter].color_hue = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void color_saturation_scale_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->color_saturation_entry, val);

    self->con.settings.color_presets[self->selected_filter].color_saturation = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void color_lightness_scale_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->color_lightness_entry, val);
    
    self->con.settings.color_presets[self->selected_filter].color_lightness = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

/*void noise_reduction_scale_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->noise_reduction_entry, val);
    self->con.settings.noise_reduction = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void noise_reduction_sharpen_scale_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    set_entry_value(self->noise_reduction_sharpen_entry, val);
    self->con.settings.noise_reduction_sharpen = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}*/
