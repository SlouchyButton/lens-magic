#include "lens-magic-window-signals.h"

void exposure_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    GString* str_val = g_string_new(NULL);
    g_string_printf(str_val, "%.2f", val);
    GtkEntryBuffer* entry_buf = gtk_entry_get_buffer(self->exposure_entry);
    gtk_entry_buffer_set_text (entry_buf, str_val->str, -1);

    self->con.settings.exposure = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void brightness_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    GString* str_val = g_string_new(NULL);
    if (val == 100 || val == -100) {
        g_string_printf(str_val, "%.0f", val);
    } else {
        g_string_printf(str_val, "%.2f", val);
    }
    GtkEntryBuffer* entry_buf = gtk_entry_get_buffer(self->brightness_entry);
    gtk_entry_buffer_set_text (entry_buf, str_val->str, -1);

    self->con.settings.brightness = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void contrast_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    GString* str_val = g_string_new(NULL);
    g_string_printf(str_val, "%.2f", val);
    GtkEntryBuffer* entry_buf = gtk_entry_get_buffer(self->contrast_entry);
    gtk_entry_buffer_set_text (entry_buf, str_val->str, -1);

    self->con.settings.contrast = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void highlights_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    GString* str_val = g_string_new(NULL);
    g_string_printf(str_val, "%.2f", val);
    GtkEntryBuffer* entry_buf = gtk_entry_get_buffer(self->highlights_entry);
    gtk_entry_buffer_set_text (entry_buf, str_val->str, -1);

    self->con.settings.highlights = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void shadows_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    GString* str_val = g_string_new(NULL);
    g_string_printf(str_val, "%.2f", val);
    GtkEntryBuffer* entry_buf = gtk_entry_get_buffer(self->shadows_entry);
    gtk_entry_buffer_set_text (entry_buf, str_val->str, -1);

    self->con.settings.shadows = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void temperature_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    GString* str_val = g_string_new(NULL);
    g_string_printf(str_val, "%.2f", val);
    GtkEntryBuffer* entry_buf = gtk_entry_get_buffer(self->temperature_entry);
    gtk_entry_buffer_set_text (entry_buf, str_val->str, -1);

    self->con.settings.temperature = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void tint_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    GString* str_val = g_string_new(NULL);
    g_string_printf(str_val, "%.2f", val);
    GtkEntryBuffer* entry_buf = gtk_entry_get_buffer(self->tint_entry);
    gtk_entry_buffer_set_text (entry_buf, str_val->str, -1);

    self->con.settings.tint = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void saturation_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    GString* str_val = g_string_new(NULL);
    g_string_printf(str_val, "%.2f", val);
    GtkEntryBuffer* entry_buf = gtk_entry_get_buffer(self->saturation_entry);
    gtk_entry_buffer_set_text (entry_buf, str_val->str, -1);

    self->con.settings.saturation = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void color_hue_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    GString* str_val = g_string_new(NULL);
    g_string_printf(str_val, "%.2f", val);
    GtkEntryBuffer* entry_buf = gtk_entry_get_buffer(self->color_hue_entry);
    gtk_entry_buffer_set_text (entry_buf, str_val->str, -1);

    self->con.settings.color_hue = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void color_saturation_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    GString* str_val = g_string_new(NULL);
    g_string_printf(str_val, "%.2f", val);
    GtkEntryBuffer* entry_buf = gtk_entry_get_buffer(self->color_saturation_entry);
    gtk_entry_buffer_set_text (entry_buf, str_val->str, -1);

    self->con.settings.color_saturation = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

void color_lightness_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    GString* str_val = g_string_new(NULL);
    g_string_printf(str_val, "%.2f", val);
    GtkEntryBuffer* entry_buf = gtk_entry_get_buffer(self->color_lightness_entry);
    gtk_entry_buffer_set_text (entry_buf, str_val->str, -1);

    self->con.settings.color_lightness = val;

    redraw_image ((GtkGLArea*)self->gl_area);
}

