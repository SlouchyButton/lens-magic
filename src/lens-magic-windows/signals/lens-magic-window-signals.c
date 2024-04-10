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


//-------------------//
// General callbacks //
//-------------------//
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
