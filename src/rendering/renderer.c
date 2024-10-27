#include "renderer.h"

void redraw_image(RendererControl* con, GtkPicture* pic) {
    cl_render(con);

    GBytes* bytes = g_bytes_new(con->rendered_image_data, con->image_data_size);
    g_object_unref (con->tex_rendered);
    con->tex_rendered = gdk_memory_texture_new(con->width, con->height,
                                                con->bit_depth == 16 ? GDK_MEMORY_R16G16B16A16 : GDK_MEMORY_R8G8B8A8,
                                                bytes,
                                                con->width*4);
    g_bytes_unref(bytes);
    gtk_picture_set_paintable (pic, NULL);
    gtk_picture_set_paintable (pic, GDK_PAINTABLE(con->tex_rendered));
}