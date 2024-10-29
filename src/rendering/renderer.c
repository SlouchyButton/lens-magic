#include "renderer.h"

void redraw_image(RendererControl* con, GtkPicture* pic) {
    cl_render(con);

    GBytes* bytes = g_bytes_new(con->preview_image_data, con->preview_width * con->preview_height * 4);
    g_object_unref (con->tex_rendered);
    con->tex_rendered = gdk_memory_texture_new(con->preview_width, con->preview_height,
                                                GDK_MEMORY_R8G8B8A8,
                                                bytes,
                                                con->preview_width*4);
    g_bytes_unref(bytes);
    gtk_picture_set_paintable (pic, NULL);
    gtk_picture_set_paintable (pic, GDK_PAINTABLE(con->tex_rendered));
}