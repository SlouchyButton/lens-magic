#include <epoxy/gl.h>

#include "config.h"
#include "lens-magic-window.h"
#include "signals/lens-magic-window-signals.h"
#include "rendering/image-processing.h"

G_DEFINE_FINAL_TYPE(LensMagicWindow, lens_magic_window, ADW_TYPE_APPLICATION_WINDOW)


void ogl_init(LensMagicWindow* self);

void open_file(GtkButton* btn, LensMagicWindow *self);
void on_open_response(GObject *source_object, GAsyncResult *res, LensMagicWindow *self);
void export_file(GtkButton* btn, LensMagicWindow *self);
void on_export_response(GObject *source_object, GAsyncResult *res, LensMagicWindow *self);

static void lens_magic_window_class_init(LensMagicWindowClass *klass) {
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

	gtk_widget_class_set_template_from_resource (widget_class, "/com/alesnezbeda/lens-magic/gtk/lens-magic-window.ui");
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, header_bar);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, testbox);

    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, processing_spinner);

    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, open_file_button);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, export_button);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, original_switch);
    
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, filter_red_button);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, filter_green_button);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, filter_blue_button);

    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, exposure_switch);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, brightness_switch);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, contrast_switch);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, highlights_switch);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, shadows_switch);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, temperature_switch);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, tint_switch);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, saturation_switch);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, noise_reduction_switch);

    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, exposure_scale);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, brightness_scale);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, contrast_scale);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, highlights_scale);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, shadows_scale);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, temperature_scale);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, tint_scale);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, saturation_scale);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, color_hue_scale);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, color_saturation_scale);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, color_lightness_scale);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, noise_reduction_scale);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, noise_reduction_sharpen_scale);

    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, exposure_entry);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, brightness_entry);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, contrast_entry);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, highlights_entry);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, shadows_entry);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, temperature_entry);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, tint_entry);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, saturation_entry);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, color_hue_entry);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, color_saturation_entry);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, color_lightness_entry);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, noise_reduction_entry);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, noise_reduction_sharpen_entry);
}


static void lens_magic_window_init(LensMagicWindow *self) {
    gtk_widget_init_template (GTK_WIDGET (self));

    // TODO: Don't particularly like having to do this, initializing widgets directly into it
    // would be better, but haven't found a way of doing it with 
    // gtk_widget_class_bind_template_child 
    self->elements.exposure = (AdjustmentElements) { 
        self, self->exposure_switch, self->exposure_scale, 
        self->exposure_entry, &self->con.settings.exposure
    };
    self->elements.brightness = (AdjustmentElements) { 
        self, self->brightness_switch, self->brightness_scale, 
        self->brightness_entry, &self->con.settings.brightness
    };
    self->elements.contrast = (AdjustmentElements) { 
        self, self->contrast_switch, self->contrast_scale, 
        self->contrast_entry, &self->con.settings.contrast
    };
    self->elements.highlights = (AdjustmentElements) { 
        self, self->highlights_switch, self->highlights_scale, 
        self->highlights_entry, &self->con.settings.highlights
    };
    self->elements.shadows = (AdjustmentElements) { 
        self, self->shadows_switch, self->shadows_scale, 
        self->shadows_entry, &self->con.settings.shadows
    };
    self->elements.temperature = (AdjustmentElements) { 
        self, self->temperature_switch, self->temperature_scale, 
        self->temperature_entry, &self->con.settings.temperature
    };
    self->elements.tint = (AdjustmentElements) { 
        self, self->tint_switch, self->tint_scale, 
        self->tint_entry, &self->con.settings.tint
    };
    self->elements.saturation = (AdjustmentElements) { 
        self, self->saturation_switch, self->saturation_scale, 
        self->saturation_entry, &self->con.settings.saturation
    };
    self->elements.noise_reduction = (AdjustmentElements) { 
        self, self->noise_reduction_switch, self->noise_reduction_scale, 
        self->noise_reduction_entry, &self->con.settings.noise_reduction
    };
    self->elements.noise_reduction_sharpen = (AdjustmentElements) { 
        self, self->noise_reduction_switch, self->noise_reduction_sharpen_scale, 
        self->noise_reduction_sharpen_entry, &self->con.settings.noise_reduction_sharpen
    };

    /*gtk_scale_add_mark (self->exposure_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->brightness_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->contrast_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->highlights_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->shadows_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->temperature_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->tint_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->saturation_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->color_hue_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->color_saturation_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->color_lightness_scale, 0, GTK_POS_BOTTOM, NULL);*/

    g_signal_connect(self->open_file_button, "clicked", (GCallback) open_file, self);
    g_signal_connect(self->export_button, "clicked", (GCallback) export_file, self);
    g_signal_connect(self->original_switch, "state-set", (GCallback) original_switch_state_set, self);

    g_signal_connect(self->exposure_switch, "state-set", (GCallback) adj_switch_state_set, &self->elements.exposure);
    g_signal_connect(self->brightness_switch, "state-set", (GCallback) adj_switch_state_set, &self->elements.brightness);
    g_signal_connect(self->contrast_switch, "state-set", (GCallback) adj_switch_state_set, &self->elements.contrast);
    g_signal_connect(self->highlights_switch, "state-set", (GCallback) adj_switch_state_set, &self->elements.highlights);
    g_signal_connect(self->shadows_switch, "state-set", (GCallback) adj_switch_state_set, &self->elements.shadows);
    g_signal_connect(self->temperature_switch, "state-set", (GCallback) adj_switch_state_set, &self->elements.temperature);
    g_signal_connect(self->tint_switch, "state-set", (GCallback) adj_switch_state_set, &self->elements.tint);
    g_signal_connect(self->saturation_switch, "state-set", (GCallback) adj_switch_state_set, &self->elements.saturation);
    g_signal_connect(self->noise_reduction_switch, "state-set", (GCallback) adj_switch_state_set, &self->elements.noise_reduction);

    g_signal_connect(self->exposure_scale, "value-changed", (GCallback) adj_scale_change, &self->elements.exposure);
    g_signal_connect(self->brightness_scale, "value-changed", (GCallback) adj_scale_change, &self->elements.brightness);
    g_signal_connect(self->contrast_scale, "value-changed", (GCallback) adj_scale_change, &self->elements.contrast);
    g_signal_connect(self->highlights_scale, "value-changed", (GCallback) adj_scale_change, &self->elements.highlights);
    g_signal_connect(self->shadows_scale, "value-changed", (GCallback) adj_scale_change, &self->elements.shadows);
    g_signal_connect(self->temperature_scale, "value-changed", (GCallback) adj_scale_change, &self->elements.temperature);
    g_signal_connect(self->tint_scale, "value-changed", (GCallback) adj_scale_change, &self->elements.tint);
    g_signal_connect(self->saturation_scale, "value-changed", (GCallback) adj_scale_change, &self->elements.saturation);
    g_signal_connect(self->noise_reduction_scale, "value-changed", (GCallback) adj_scale_change, &self->elements.noise_reduction);
    g_signal_connect(self->noise_reduction_sharpen_scale, "value-changed", (GCallback) adj_scale_change, &self->elements.noise_reduction_sharpen);

    g_signal_connect(self->color_hue_scale, "value-changed", (GCallback) color_hue_scale_change, self);
    g_signal_connect(self->color_saturation_scale, "value-changed", (GCallback) color_saturation_scale_change, self);
    g_signal_connect(self->color_lightness_scale, "value-changed", (GCallback) color_lightness_scale_change, self);
    
    g_signal_connect(self->filter_red_button, "clicked", (GCallback) filter_red_button_clicked, self);
    g_signal_connect(self->filter_green_button, "clicked", (GCallback) filter_green_button_clicked, self);
    g_signal_connect(self->filter_blue_button, "clicked", (GCallback) filter_blue_button_clicked, self);

    self->selected_filter = 0;

    memset(&self->con.settings, 0, sizeof(self->con.settings));

    self->gl_area = gtk_gl_area_new();
    gtk_widget_set_hexpand (self->gl_area, TRUE);
    gtk_widget_set_vexpand (self->gl_area, TRUE);
    gtk_widget_set_size_request (self->gl_area, 100, 200);
    gtk_box_append (GTK_BOX (self->testbox), self->gl_area);
    gtk_gl_area_set_required_version(GTK_GL_AREA (self->gl_area), 3, 3);
    self->con.ogl_frame = self->gl_area;

    g_signal_connect(self->gl_area, "realize", G_CALLBACK (realize), &self->con);
    g_signal_connect(self->gl_area, "unrealize", G_CALLBACK (unrealize), NULL);
    g_signal_connect(self->gl_area, "render", G_CALLBACK (render), &self->con);

    redraw_image((GtkGLArea*)self->gl_area);
}

void open_file(GtkButton* btn, LensMagicWindow* self) {
    GtkFileDialog *fd = gtk_file_dialog_new();

    gtk_file_dialog_open(fd, (GtkWindow *)self, NULL, (GAsyncReadyCallback) on_open_response, self);
}

void on_open_response(GObject *source_object, GAsyncResult *res, LensMagicWindow *self) {
    GFile *file = gtk_file_dialog_open_finish((GtkFileDialog *)source_object, res, NULL);
    if (file == NULL)
    {
        return;
    }
    //g_autofree char *file_name = g_file_get_basename(file);
    char *path = g_file_get_path(file);

    self->con.original_path = path;

    gtk_widget_set_visible((GtkWidget*)self->processing_spinner, TRUE);
    gtk_widget_set_sensitive((GtkWidget*)self->open_file_button, FALSE);

    g_thread_new("image_processing", process_image, (gpointer)self);

    //refresh_textures(&self->con); 
    //redraw_image((GtkGLArea*)self->gl_area);
}

void export_file(GtkButton* btn, LensMagicWindow* self) {
    GtkFileDialog *fd = gtk_file_dialog_new();

    gtk_file_dialog_save(fd, (GtkWindow *)self, NULL, (GAsyncReadyCallback) on_export_response, self);
}

void on_export_response(GObject *source_object, GAsyncResult *res, LensMagicWindow *self) {
    GFile *file = gtk_file_dialog_save_finish((GtkFileDialog *)source_object, res, NULL);
    if (file == NULL)
    {
        return;
    }
    //g_autofree char* file_name = g_file_get_basename(file);
    char* path = g_file_get_path(file);
    self->con.export_path = path;
    self->con.export_pending = true;
    redraw_image((GtkGLArea*)self->gl_area);
}
