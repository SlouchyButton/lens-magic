#include <libraw.h>

#include "image-processing.h"

/**
 * Wrapper function to be called by g_idle_add. 
 * Calls redraw_image from image-renderer. Also hides spinner and changes button sensitivity.
 *
 * @param   data     Pointer to LensMagicWindow struct
 */
gboolean render_processed_image(gpointer data) {
    LensMagicWindow* self = (LensMagicWindow*) data;
    redraw_image((GtkGLArea*)self->gl_area);
    gtk_widget_set_visible((GtkWidget*)self->processing_spinner, FALSE);
    gtk_widget_set_sensitive((GtkWidget*)self->open_file_button, TRUE);
    return FALSE;
}

gpointer process_image(gpointer data) {
    LensMagicWindow* self = (LensMagicWindow*) data;
    libraw_data_t* libraw_handle = libraw_init(0);
    if (!libraw_handle) {
        fprintf(stderr, "Cannot create libraw handle\n");
        return NULL;
    }

    libraw_handle->params.use_camera_wb = 1;
    libraw_handle->params.use_auto_wb = 0;
    libraw_handle->params.no_auto_bright = 1;
    libraw_handle->params.output_bps = 16;
    libraw_handle->params.adjust_maximum_thr = 0.0;
    libraw_handle->params.user_qual = 12;

    int libraw_error = 0;
    if ((libraw_error = libraw_open_file(libraw_handle, self->con.original_path)) == 0) {
        printf("Processing %s %s %s\n", self->con.original_path, libraw_handle->idata.make,
           libraw_handle->idata.model);
        libraw_unpack(libraw_handle);
        libraw_dcraw_process(libraw_handle);
        g_autofree libraw_processed_image_t* image = libraw_dcraw_make_mem_image(libraw_handle, NULL);

        free(self->con.image_data);
        self->con.image_data = calloc(image->width * image->height * 3, sizeof(uint16_t));
        memcpy(self->con.image_data, image->data, image->width * image->height * 3 * sizeof(uint16_t));

        self->con.original_width = image->width;
        self->con.original_height = image->height;
        self->con.bit_depth = image->bits;
    } else {
        fprintf(stderr, "Couldn't open file using libraw (errno: %d), trying gdk fallback\n",
            libraw_error);
        GError* err = NULL;
        g_autofree GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file(self->con.original_path, &err);
        if (err) {
            fprintf(stderr, "Couldn't open file using gdk fallback (err: %s), not continuing\n",
                err->message);
            g_idle_add(render_processed_image, data);
            return NULL;
        }
        guint len = 0;
        guchar* pix = gdk_pixbuf_get_pixels_with_length (pixbuf, &len);

        free(self->con.image_data);
        self->con.image_data = calloc(len, sizeof(guchar));
        memcpy(self->con.image_data, pix, len * sizeof(guchar));
        self->con.original_width = gdk_pixbuf_get_width(pixbuf);
        self->con.original_height = gdk_pixbuf_get_height(pixbuf);
        self->con.bit_depth = 8;
    }
    libraw_close(libraw_handle);


    if (self->con.original_height > self->con.max_tex_size || self->con.original_width > self->con.max_tex_size) {
        self->con.width = self->con.max_tex_size;
        self->con.height = ((gdouble)self->con.original_height/self->con.original_width)*self->con.max_tex_size;
        if (self->con.height > self->con.max_tex_size) {
            self->con.width = ((gdouble)self->con.original_width/self->con.original_height)*self->con.max_tex_size;
            self->con.height = self->con.max_tex_size;
        }
        printf("Image is bigger than what OGL can render new dimensions: %dx%d\n", self->con.width, self->con.height);
    }

    if (self->con.original_height > 1080) {
        self->con.preview_height = self->con.original_height / 4;
        self->con.preview_width = self->con.original_width / 4;
    } else {
        self->con.preview_height = self->con.original_height;
        self->con.preview_width = self->con.original_width;
    }

    self->con.texture_refresh_pending = true;

    g_idle_add(render_processed_image, data);

    return NULL;
}