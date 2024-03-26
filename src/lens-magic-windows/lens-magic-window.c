#include <epoxy/gl.h>
#include <libraw.h>

#include "config.h"
#include "lens-magic-window.h"
#include "signals/lens-magic-window-signals.h"

G_DEFINE_FINAL_TYPE (LensMagicWindow, lens_magic_window, ADW_TYPE_APPLICATION_WINDOW)


void ogl_init(LensMagicWindow* self);

void open_file(GtkButton* btn, LensMagicWindow *self);
void on_open_response(GObject *source_object, GAsyncResult *res, LensMagicWindow *self);
void export_file(GtkButton* btn, LensMagicWindow *self);
void on_export_response(GObject *source_object, GAsyncResult *res, LensMagicWindow *self);

static void lens_magic_window_class_init (LensMagicWindowClass *klass)
{
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

	gtk_widget_class_set_template_from_resource (widget_class, "/org/slouchybutton/LensMagic/gtk/lens-magic-window.ui");
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, header_bar);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, testbox);

    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, open_file_button);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, export_button);

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
}


static void lens_magic_window_init (LensMagicWindow *self)
{
    gtk_widget_init_template (GTK_WIDGET (self));

    /*gtk_scale_add_mark (self->exposure_scale, 1, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->brightness_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->contrast_scale, 1, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->highlights_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->shadows_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->temperature_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->tint_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->saturation_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->color_hue_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->color_saturation_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->color_lightness_scale, 1, GTK_POS_BOTTOM, NULL);*/

    g_signal_connect (self->exposure_scale, "value-changed", (GCallback) exposure_change, self);
    g_signal_connect (self->brightness_scale, "value-changed", (GCallback) brightness_change, self);
    g_signal_connect (self->contrast_scale, "value-changed", (GCallback) contrast_change, self);
    g_signal_connect (self->highlights_scale, "value-changed", (GCallback) highlights_change, self);
    g_signal_connect (self->shadows_scale, "value-changed", (GCallback) shadows_change, self);
    g_signal_connect (self->temperature_scale, "value-changed", (GCallback) temperature_change, self);
    g_signal_connect (self->tint_scale, "value-changed", (GCallback) tint_change, self);
    g_signal_connect (self->saturation_scale, "value-changed", (GCallback) saturation_change, self);
    g_signal_connect (self->color_hue_scale, "value-changed", (GCallback) color_hue_change, self);
    g_signal_connect (self->color_saturation_scale, "value-changed", (GCallback) color_saturation_change, self);
    g_signal_connect (self->color_lightness_scale, "value-changed", (GCallback) color_lightness_change, self);

    g_signal_connect (self->open_file_button, "clicked", (GCallback) open_file, self);
    g_signal_connect (self->export_button, "clicked", (GCallback) export_file, self);

    self->con.settings.exposure = 1;
    self->con.settings.brightness = 0;
    self->con.settings.contrast = 1;
    self->con.settings.highlights = 0;
    self->con.settings.shadows = 0;

    self->gl_area = gtk_gl_area_new();
    gtk_widget_set_hexpand (self->gl_area, TRUE);
    gtk_widget_set_vexpand (self->gl_area, TRUE);
    gtk_widget_set_size_request (self->gl_area, 100, 200);
    gtk_box_append (GTK_BOX (self->testbox), self->gl_area);
    self->con.ogl_frame = self->gl_area;

    /* We need to initialize and free GL resources, so we use
    * the realize and unrealize signals on the widget
    */
    g_signal_connect (self->gl_area, "realize", G_CALLBACK (realize), &self->con);
    g_signal_connect (self->gl_area, "unrealize", G_CALLBACK (unrealize), NULL);

    /* The main "draw" call for GtkGLArea */
    g_signal_connect (self->gl_area, "render", G_CALLBACK (render), &self->con);

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
    g_autofree char *file_name = g_file_get_basename(file);
    g_autofree char *path = g_file_get_path(file);

    libraw_data_t* libraw_handle = libraw_init(0);
    if (!libraw_handle) {
        fprintf(stderr, "Cannot create libraw handle\n");
        return;
    }

    libraw_handle->params.use_camera_wb = 1;
    libraw_handle->params.use_auto_wb = 0;
    libraw_handle->params.no_auto_bright = 1;
    libraw_handle->params.output_bps = 16;
    libraw_handle->params.adjust_maximum_thr = 0.0;
    libraw_handle->params.user_qual = 12;


    if (libraw_open_file(libraw_handle, path) == 0) {
        printf("Processing %s %s %s\n", path, libraw_handle->idata.make,
           libraw_handle->idata.model);
        libraw_unpack(libraw_handle);
        libraw_dcraw_process(libraw_handle);
        g_autofree libraw_processed_image_t* image = libraw_dcraw_make_mem_image(libraw_handle, NULL);

        free(self->con.image_data);
        self->con.image_data = calloc(image->width * image->height * 3, sizeof(uint16_t));
        memcpy(self->con.image_data, image->data, image->width * image->height * 3 * sizeof(uint16_t));

        self->con.width = image->width;
        self->con.height = image->height;
        self->con.bit_depth = image->bits;
    } else {
        fprintf(stderr, "Couldn't open file using libraw, trying gdk fallback\n");
        g_autofree GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file(path, NULL);
        guint len = 0;
        guchar* pix = gdk_pixbuf_get_pixels_with_length (pixbuf, &len);

        free(self->con.image_data);
        self->con.image_data = calloc(len, sizeof(guchar));
        memcpy(self->con.image_data, pix, len * sizeof(guchar));
        self->con.width = gdk_pixbuf_get_width(pixbuf);
        self->con.height = gdk_pixbuf_get_height(pixbuf);
        self->con.bit_depth = 8;
    }
    libraw_close(libraw_handle);

    if (self->con.height > 1080) {
        self->con.preview_height = self->con.height / 4;
        self->con.preview_width = self->con.width / 4;
    } else {
        self->con.preview_height = self->con.height;
        self->con.preview_width = self->con.width;
    }

    self->con.texture_refresh_pending = true;

    //refresh_textures(&self->con); 
    redraw_image((GtkGLArea*)self->gl_area);
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


