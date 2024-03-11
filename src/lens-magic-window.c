
#include "config.h"

#include "lens-magic-window.h"

#include "color-utils.h"
#include "image-renderer.h"
#include <epoxy/gl.h>

struct _LensMagicWindow
{
    AdwApplicationWindow parent_instance;

    Preset settings;
    RendererControl con;

    /* Template widgets */
    GtkHeaderBar    *header_bar;
    GtkButton       *open_file_button;


    GtkWidget       *testbox;
    GtkWidget       *gl_area;
    GdkPixbuf       *pxb_original;
    GdkTexture      *tex_rendered;

    GtkScale        *exposure_scale;
    GtkScale        *brightness_scale;
    GtkScale        *contrast_scale;
    GtkScale        *highlights_scale;
    GtkScale        *shadows_scale;
    GtkScale        *temperature_scale;
    GtkScale        *tint_scale;
    GtkScale        *saturation_scale;
    GtkScale        *color_hue_scale;
    GtkScale        *color_saturation_scale;
    GtkScale        *color_lightness_scale;

    GtkEntry        *exposure_entry;
    GtkEntry        *brightness_entry;
    GtkEntry        *contrast_entry;
    GtkEntry        *highlights_entry;
    GtkEntry        *shadows_entry;
    GtkEntry        *temperature_entry;
    GtkEntry        *tint_entry;
    GtkEntry        *saturation_entry;
    GtkEntry        *color_hue_entry;
    GtkEntry        *color_saturation_entry;
    GtkEntry        *color_lightness_entry;

};

G_DEFINE_FINAL_TYPE (LensMagicWindow, lens_magic_window, ADW_TYPE_APPLICATION_WINDOW)

void exposure_change(GtkRange* range, LensMagicWindow *self);
void brightness_change(GtkRange* range, LensMagicWindow *self);
void contrast_change(GtkRange* range, LensMagicWindow *self);
void highlights_change(GtkRange* range, LensMagicWindow *self);
void shadows_change(GtkRange* range, LensMagicWindow *self);
void temperature_change(GtkRange* range, LensMagicWindow *self);
void tint_change(GtkRange* range, LensMagicWindow *self);
void saturation_change(GtkRange* range, LensMagicWindow *self);
void color_hue_change(GtkRange* range, LensMagicWindow *self);
void color_saturation_change(GtkRange* range, LensMagicWindow *self);
void color_lightness_change(GtkRange* range, LensMagicWindow *self);
void ogl_init(LensMagicWindow* self);
void redraw_image(LensMagicWindow *self);

void open_file(LensMagicWindow *self);
void on_open_response(GObject *source_object, GAsyncResult *res, LensMagicWindow *self);

static void lens_magic_window_class_init (LensMagicWindowClass *klass)
{
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

	gtk_widget_class_set_template_from_resource (widget_class, "/org/slouchybutton/LensMagic/lens-magic-window.ui");
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, header_bar);
    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, testbox);

    gtk_widget_class_bind_template_child (widget_class, LensMagicWindow, open_file_button);

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

    gtk_scale_add_mark (self->exposure_scale, 1, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->brightness_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->contrast_scale, 1, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->highlights_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->shadows_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->temperature_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->tint_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->saturation_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->color_hue_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->color_saturation_scale, 0, GTK_POS_BOTTOM, NULL);
    gtk_scale_add_mark (self->color_lightness_scale, 1, GTK_POS_BOTTOM, NULL);

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

    self->settings.exposure = 1;
    self->settings.brightness = 0;
    self->settings.contrast = 1;
    self->settings.highlights = 0;
    self->settings.shadows = 0;

    self->pxb_original = gdk_pixbuf_new_from_file_at_size ("/home/slouchy/IMG_8191.jpg",
                                               1920, -1, NULL);
    self->pxb_original = gdk_pixbuf_add_alpha (self->pxb_original, false, 0, 0, 0);
    g_print ("Bit/sample: %d, Alpha: %d, Channels: %d\n",
             gdk_pixbuf_get_bits_per_sample (self->pxb_original),
             gdk_pixbuf_get_has_alpha (self->pxb_original),
             gdk_pixbuf_get_n_channels (self->pxb_original));

    self->tex_rendered = gdk_texture_new_for_pixbuf(self->pxb_original);

    self->con.kill_thread = false;
    self->con.pending_refresh = true;
    self->con.pending_new_picture = true;
    self->con.pxb_original = self->pxb_original;

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

    redraw_image(self);


    /*GtkWidget *gl_area = gtk_gl_area_new ();

    self->con.ogl_frame = (GtkGLArea*)gl_area;

    gtk_box_append(self->testbox, gl_area);

    g_signal_connect (gl_area, "realize", (GCallback) lolwtf, gl_area);*/

    //g_thread_new("image_renderer", renderer, &self->con);
}

void open_file(LensMagicWindow *self) {
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

    self->pxb_original = gdk_pixbuf_new_from_file_at_size (path,
                                               1920, -1, NULL);
    self->pxb_original = gdk_pixbuf_add_alpha (self->pxb_original, false, 0, 0, 0);
    g_print ("Bit/sample: %d, Alpha: %d, Channels: %d\n",
             gdk_pixbuf_get_bits_per_sample (self->pxb_original),
             gdk_pixbuf_get_has_alpha (self->pxb_original),
             gdk_pixbuf_get_n_channels (self->pxb_original));

    self->tex_rendered = gdk_texture_new_for_pixbuf(self->pxb_original);

    self->con.kill_thread = false;
    self->con.pending_refresh = true;
    self->con.pending_new_picture = true;
    self->con.pxb_original = self->pxb_original;

    refresh_textures (&self->con);
}

void redraw_image(LensMagicWindow *self) {
    /*GdkPixbuf* pxb = gdk_pixbuf_copy(self->pxb_original);

    render_pixbuf (pxb, self->settings);

    GdkTexture* tex = gdk_texture_new_for_pixbuf(pxb);
    g_object_unref (pxb);

    gtk_picture_set_paintable (self->picture, GDK_PAINTABLE(tex));
    g_object_unref (self->tex_rendered);
    self->tex_rendered = tex;*/
    self->con.settings = self->settings;
    gtk_gl_area_queue_render((GtkGLArea*)self->gl_area);

    /*g_mutex_lock (&self->con.data_mutex);
    self->con.settings = self->settings;
    self->con.pending_refresh = true;
    g_cond_signal (&self->con.data_cond);
    g_mutex_unlock (&self->con.data_mutex);*/
}

void exposure_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    GString* str_val = g_string_new(NULL);
    g_string_printf(str_val, "%.2f", val);
    GtkEntryBuffer* entry_buf = gtk_entry_get_buffer(self->exposure_entry);
    gtk_entry_buffer_set_text (entry_buf, str_val->str, -1);

    self->settings.exposure = val;

    redraw_image (self);
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

    self->settings.brightness = val;

    redraw_image (self);
}

void contrast_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    GString* str_val = g_string_new(NULL);
    g_string_printf(str_val, "%.2f", val);
    GtkEntryBuffer* entry_buf = gtk_entry_get_buffer(self->contrast_entry);
    gtk_entry_buffer_set_text (entry_buf, str_val->str, -1);

    self->settings.contrast = val;

    redraw_image (self);
}

void highlights_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    GString* str_val = g_string_new(NULL);
    g_string_printf(str_val, "%.2f", val);
    GtkEntryBuffer* entry_buf = gtk_entry_get_buffer(self->highlights_entry);
    gtk_entry_buffer_set_text (entry_buf, str_val->str, -1);

    self->settings.highlights = val;

    redraw_image (self);
}

void shadows_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    GString* str_val = g_string_new(NULL);
    g_string_printf(str_val, "%.2f", val);
    GtkEntryBuffer* entry_buf = gtk_entry_get_buffer(self->shadows_entry);
    gtk_entry_buffer_set_text (entry_buf, str_val->str, -1);

    self->settings.shadows = val;

    redraw_image (self);
}

void temperature_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    GString* str_val = g_string_new(NULL);
    g_string_printf(str_val, "%.2f", val);
    GtkEntryBuffer* entry_buf = gtk_entry_get_buffer(self->temperature_entry);
    gtk_entry_buffer_set_text (entry_buf, str_val->str, -1);

    self->settings.temperature = val;

    redraw_image (self);
}

void tint_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    GString* str_val = g_string_new(NULL);
    g_string_printf(str_val, "%.2f", val);
    GtkEntryBuffer* entry_buf = gtk_entry_get_buffer(self->tint_entry);
    gtk_entry_buffer_set_text (entry_buf, str_val->str, -1);

    self->settings.tint = val;

    redraw_image (self);
}

void saturation_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    GString* str_val = g_string_new(NULL);
    g_string_printf(str_val, "%.2f", val);
    GtkEntryBuffer* entry_buf = gtk_entry_get_buffer(self->saturation_entry);
    gtk_entry_buffer_set_text (entry_buf, str_val->str, -1);

    self->settings.saturation = val;

    redraw_image (self);
}

void color_hue_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    GString* str_val = g_string_new(NULL);
    g_string_printf(str_val, "%.2f", val);
    GtkEntryBuffer* entry_buf = gtk_entry_get_buffer(self->color_hue_entry);
    gtk_entry_buffer_set_text (entry_buf, str_val->str, -1);

    self->settings.color_hue = val;

    redraw_image (self);
}

void color_saturation_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    GString* str_val = g_string_new(NULL);
    g_string_printf(str_val, "%.2f", val);
    GtkEntryBuffer* entry_buf = gtk_entry_get_buffer(self->color_saturation_entry);
    gtk_entry_buffer_set_text (entry_buf, str_val->str, -1);

    self->settings.color_saturation = val;

    redraw_image (self);
}

void color_lightness_change(GtkRange* range, LensMagicWindow *self) {
    gdouble val = gtk_range_get_value (range);
    GString* str_val = g_string_new(NULL);
    g_string_printf(str_val, "%.2f", val);
    GtkEntryBuffer* entry_buf = gtk_entry_get_buffer(self->color_lightness_entry);
    gtk_entry_buffer_set_text (entry_buf, str_val->str, -1);

    self->settings.color_lightness = val;

    redraw_image (self);
}

