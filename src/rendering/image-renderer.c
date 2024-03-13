#include "image-renderer.h"
#include <unistd.h>
#include <epoxy/gl.h>
#include <epoxy/glx.h>

#include "shaders/shaders-source.h"

GLuint create_shader(const GLchar* const* shader_source, GLuint type) {
    int status;
    char status_string[512];
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, shader_source, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(shader, 512, NULL, status_string);
        printf("Fragment shader compilation failed: %s\n", status_string);
    }
    return shader;
}

GLuint create_program(const GLuint vertex_shader, const GLuint fragment_shader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    return program;
}

void prepare_programs(RendererControl* con) {
    GLuint vertex_shader = create_shader(&default_vs, GL_VERTEX_SHADER);
    GLuint flipped_vertex_shader  = create_shader(&flipped_vs, GL_VERTEX_SHADER);
    GLuint plain_shader = create_shader(&default_fs, GL_FRAGMENT_SHADER);
    GLuint exposure_shader = create_shader(&exposure_fs, GL_FRAGMENT_SHADER);
    GLuint brightness_shader = create_shader(&brightness_fs, GL_FRAGMENT_SHADER);
    GLuint contrast_shader = create_shader(&contrast_fs, GL_FRAGMENT_SHADER);
    GLuint temperature_shader = create_shader(&temperature_fs, GL_FRAGMENT_SHADER);
    GLuint tint_shader = create_shader(&tint_fs, GL_FRAGMENT_SHADER);
    GLuint saturation_shader = create_shader(&saturation_fs, GL_FRAGMENT_SHADER);
    GLuint highlights_shader = create_shader(&highlights_fs, GL_FRAGMENT_SHADER);
    GLuint shadows_shader = create_shader(&shadows_fs, GL_FRAGMENT_SHADER);

    con->programs.plain = create_program(flipped_vertex_shader, plain_shader);
    con->programs.exposure = create_program(vertex_shader, exposure_shader);
    con->programs.brightness = create_program(vertex_shader, brightness_shader);
    con->programs.contrast = create_program(vertex_shader, contrast_shader);
    con->programs.temperature = create_program(vertex_shader, temperature_shader);
    con->programs.tint = create_program(vertex_shader, tint_shader);
    con->programs.saturation = create_program(vertex_shader, saturation_shader);
    con->programs.highlights = create_program(vertex_shader, highlights_shader);
    con->programs.shadows = create_program(vertex_shader, shadows_shader);

    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex_shader);
    glDeleteShader(flipped_vertex_shader);
    glDeleteShader(plain_shader);
    glDeleteShader(exposure_shader);
    glDeleteShader(brightness_shader);
    glDeleteShader(contrast_shader);
    glDeleteShader(temperature_shader);
    glDeleteShader(tint_shader);
    glDeleteShader(saturation_shader);
    glDeleteShader(highlights_shader);
    glDeleteShader(shadows_shader);
}

void prepare_textures(RendererControl* con) {
    // Initialize Base texture - this will contain original image
    glGenTextures(1, &con->tex_base);
    glBindTexture(GL_TEXTURE_2D, con->tex_base);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    //upload_texture (con);

    // FB 1 Texture
    glBindFramebuffer(GL_FRAMEBUFFER, con->fb1);
        glGenTextures(1, &con->tex_fb1);
        glBindTexture(GL_TEXTURE_2D, con->tex_fb1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, con->tex_fb1, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // FB 2 Texture
    glBindFramebuffer(GL_FRAMEBUFFER, con->fb2);
        glGenTextures(1, &con->tex_fb2);
        glBindTexture(GL_TEXTURE_2D, con->tex_fb2);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0,0, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, con->tex_fb2, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void refresh_textures(RendererControl* con) {
    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenFramebuffers(1, &con->fb1);
    glGenFramebuffers(1, &con->fb2);*/

    //glGenTextures(1, &con->tex_base);

    glBindTexture(GL_TEXTURE_2D, con->tex_base);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, con->width, con->height, 0, GL_RGB, 
            con->bit_depth == 16 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_BYTE, con->image_data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, con->fb1);
        //glGenTextures(1, &con->tex_fb1);
        glBindTexture(GL_TEXTURE_2D, con->tex_fb1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, con->width, con->height, 0, GL_RGB, 
                GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, con->tex_fb1, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    glBindFramebuffer(GL_FRAMEBUFFER, con->fb2);
        //glGenTextures(1, &con->tex_fb2);
        glBindTexture(GL_TEXTURE_2D, con->tex_fb2);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, con->width, con->height, 0, GL_RGB, 
                GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, con->tex_fb2, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/* We need to set up our state when we realize the GtkGLArea widget */
void realize(GtkWidget *widget, RendererControl* con) {
    gtk_gl_area_make_current (GTK_GL_AREA (widget));

    if (gtk_gl_area_get_error (GTK_GL_AREA (widget)) != NULL) {
        return;
    }

    prepare_programs(con);

    float vertices[] = {
        -3.0f, -1.0f, 0.0f,
        3.0f, -1.0f, 0.0f,
        0.0f,  3.0f, 0.0f
    };

    // Indices for a quad
    unsigned int indices[] = {
        0, 1, 2,
        /*0, 1, 3, // First triangle
        1, 2, 3  // Second triangle*/
    };

    // Generate and bind Vertex Array Object (VAO) and Vertex Buffer Object (VBO)
    glGenVertexArrays(1, &con->VAO);
    glGenBuffers(1, &con->VBO);
    glGenBuffers(1, &con->EBO);

    // Bind VAO
    glBindVertexArray(con->VAO);
        // Bind VBO and set vertex data
        glBindBuffer(GL_ARRAY_BUFFER, con->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Bind EBO and set index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, con->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Set vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Unbind VAO, VBO, and EBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenFramebuffers(1, &con->fb1);
    glGenFramebuffers(1, &con->fb2);

    prepare_textures(con);

    printf("Initialized OpenGL\n");
}

/* We should tear down the state when unrealizing */
void unrealize(GtkWidget *widget) {
    gtk_gl_area_make_current (GTK_GL_AREA (widget));

    if (gtk_gl_area_get_error (GTK_GL_AREA (widget)) != NULL) {
        return;
    }

    //TODO: delete everything and free it
}

void render_fb(GLuint target_fb, GLuint vao, GLuint source_texture, GLuint program, gdouble value) {
    // Prepare FB 1 and set base texture as input
    glBindFramebuffer(GL_FRAMEBUFFER, target_fb);
    glBindTexture(GL_TEXTURE_2D, source_texture);
    // Prepare program
    glUseProgram(program);
    glUniform1f(glGetUniformLocation(program, "value"), value);
    // Render
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}

gboolean render(GtkGLArea* area, GdkGLContext* context, RendererControl* con) {
    if (gtk_gl_area_get_error (area) != NULL)
        return FALSE;

    if (con->image_data == NULL)
        return FALSE;

    int width = con->width;
    int height = con->height;
    int widget_width = gtk_widget_get_width((GtkWidget*)area);
    int widget_height = gtk_widget_get_height((GtkWidget*)area);

    // Calculate rendering dimensions to fit widget and keep image aspect ratio
    int target_width = widget_width;
    int target_height = ((gdouble)height/width)*widget_width;
    if (target_height > widget_height) {
        target_width = ((gdouble)width/height)*widget_height;
        target_height = widget_height;
    }

    glViewport(0, 0, width, height);

    render_fb(con->fb1, con->VAO, con->tex_base, con->programs.temperature, con->settings.temperature);
    render_fb(con->fb2, con->VAO, con->tex_fb1, con->programs.exposure, con->settings.exposure);
    render_fb(con->fb1, con->VAO, con->tex_fb2, con->programs.brightness, con->settings.brightness);
    render_fb(con->fb2, con->VAO, con->tex_fb1, con->programs.contrast, con->settings.contrast);
    render_fb(con->fb1, con->VAO, con->tex_fb2, con->programs.tint, con->settings.tint);
    render_fb(con->fb2, con->VAO, con->tex_fb1, con->programs.saturation, con->settings.saturation);
    render_fb(con->fb1, con->VAO, con->tex_fb2, con->programs.highlights, con->settings.highlights);
    render_fb(con->fb2, con->VAO, con->tex_fb1, con->programs.shadows, con->settings.shadows);

    // Prepare GTK FB, set FB 2's texture as input and set rendering dimensions based on widget size
    gtk_gl_area_attach_buffers(area);
    glBindTexture(GL_TEXTURE_2D, con->tex_fb2);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glViewport(0, 0, target_width, target_height); // widget size
    // Prepare basic program that flips the view
    glUseProgram(con->programs.plain);
    // Final render to GTK Widget
    glBindVertexArray(con->VAO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

    // Flush the contents of the pipeline
    glFlush();

    return true;
}

gboolean export(RendererControl* con, char* path) {
    int width = con->width;
    int height = con->height;

    glViewport(0, 0, width, height);

    render_fb(con->fb1, con->VAO, con->tex_base, con->programs.temperature, con->settings.temperature);
    render_fb(con->fb2, con->VAO, con->tex_fb1, con->programs.exposure, con->settings.exposure);
    render_fb(con->fb1, con->VAO, con->tex_fb2, con->programs.brightness, con->settings.brightness);
    render_fb(con->fb2, con->VAO, con->tex_fb1, con->programs.contrast, con->settings.contrast);
    render_fb(con->fb1, con->VAO, con->tex_fb2, con->programs.tint, con->settings.tint);
    render_fb(con->fb2, con->VAO, con->tex_fb1, con->programs.saturation, con->settings.saturation);
    render_fb(con->fb1, con->VAO, con->tex_fb2, con->programs.highlights, con->settings.highlights);
    render_fb(con->fb2, con->VAO, con->tex_fb1, con->programs.shadows, con->settings.shadows);

    g_autofree uint8_t* fb_data = calloc(width*height, sizeof(uint8_t)*3);

    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, fb_data);

    // Flush the contents of the pipeline
    glFlush();

    GdkPixbuf* pxbuf = gdk_pixbuf_new_from_data(fb_data, GDK_COLORSPACE_RGB, false, 8, width, height, width*3, NULL, NULL);

    gdk_pixbuf_save(pxbuf, path, "jpeg", NULL, "quality", "100", NULL);


    return true;
}

void redraw_image(GtkGLArea* gl_area) {
    gtk_gl_area_queue_render(gl_area);
}

