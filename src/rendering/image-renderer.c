#include "image-renderer.h"
#include <unistd.h>
#include <epoxy/gl.h>
#include <epoxy/glx.h>

#include "shaders/shaders-source.h"

// Copied from OpenGL documentation - handles logging OpenGL messages into stderr
void GLAPIENTRY MessageCallback( GLenum source, GLenum type, GLuint id, GLenum severity, 
        GLsizei length, const GLchar* message, const void* userParam) {
    
    if (type != GL_DEBUG_TYPE_ERROR)
        return;
    
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", 
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}

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

    GLuint color_hue_shader = create_shader(&color_hue_fs, GL_FRAGMENT_SHADER);
    GLuint color_saturation_shader = create_shader(&color_saturation_fs, GL_FRAGMENT_SHADER);
    GLuint color_lightness_shader = create_shader(&color_lightness_fs, GL_FRAGMENT_SHADER);

    GLuint denoie_shader = create_shader(&smart_denoise_fs, GL_FRAGMENT_SHADER);

    con->programs.plain = create_program(flipped_vertex_shader, plain_shader);
    con->programs.exposure = create_program(vertex_shader, exposure_shader);
    con->programs.brightness = create_program(vertex_shader, brightness_shader);
    con->programs.contrast = create_program(vertex_shader, contrast_shader);
    con->programs.temperature = create_program(vertex_shader, temperature_shader);
    con->programs.tint = create_program(vertex_shader, tint_shader);
    con->programs.saturation = create_program(vertex_shader, saturation_shader);
    con->programs.highlights = create_program(vertex_shader, highlights_shader);
    con->programs.shadows = create_program(vertex_shader, shadows_shader);
    con->programs.denoise = create_program(vertex_shader, denoie_shader);

    con->programs.color_hue = create_program(vertex_shader, color_hue_shader);
    con->programs.color_saturation = create_program(vertex_shader, color_saturation_shader);
    con->programs.color_lightness = create_program(vertex_shader, color_lightness_shader);

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
    glDeleteShader(color_hue_shader);
    glDeleteShader(color_saturation_shader);
    glDeleteShader(color_lightness_shader);
    glDeleteShader(denoie_shader);
}

void prepare_textures(RendererControl* con) {
    // Initialize Base texture - this will contain original image
    glGenTextures(1, &con->tex_base);
    glBindTexture(GL_TEXTURE_2D, con->tex_base);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

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

    glBindFramebuffer(GL_FRAMEBUFFER, con->preview_fb1);
        glGenTextures(1, &con->preview_tex_fb1);
        glBindTexture(GL_TEXTURE_2D, con->preview_tex_fb1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, con->preview_tex_fb1, 0);
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

    glBindFramebuffer(GL_FRAMEBUFFER, con->preview_fb2);
        glGenTextures(1, &con->preview_tex_fb2);
        glBindTexture(GL_TEXTURE_2D, con->preview_tex_fb2);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0,0, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, con->preview_tex_fb2, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void refresh_textures(RendererControl* con) {
    // TODO: We can't just imply that RGB8 and RGB16 are only formats, do it correctly, else it
    // won't render correctly
    glBindTexture(GL_TEXTURE_2D, con->tex_base);
        glTexImage2D(GL_TEXTURE_2D, 0, con->bit_depth == 16 ? GL_RGB16 : GL_RGB8, 
            con->width, con->height, 0, GL_RGB, 
            con->bit_depth == 16 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_BYTE, con->image_data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, con->fb1);
        glBindTexture(GL_TEXTURE_2D, con->tex_fb1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, con->width, con->height, 0, GL_RGB, 
                GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, con->fb2);
        glBindTexture(GL_TEXTURE_2D, con->tex_fb2);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, con->width, con->height, 0, GL_RGB, 
                GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, con->preview_fb1);
        glBindTexture(GL_TEXTURE_2D, con->preview_tex_fb1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, con->preview_width, con->preview_height, 0, GL_RGB, 
                GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, con->preview_fb2);
        glBindTexture(GL_TEXTURE_2D, con->preview_tex_fb2);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, con->preview_width, con->preview_height, 0, GL_RGB, 
                GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void realize(GtkWidget *widget, RendererControl* con) {
    #ifndef NDEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(MessageCallback, 0);
    #endif

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
        0, 1, 2, // Use only one triangle that is streched over the screen
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
    glGenFramebuffers(1, &con->preview_fb1);
    glGenFramebuffers(1, &con->preview_fb2);

    prepare_textures(con);

    printf("Initialized OpenGL\n");
}

void unrealize(GtkWidget *widget) {
    gtk_gl_area_make_current (GTK_GL_AREA (widget));

    if (gtk_gl_area_get_error (GTK_GL_AREA (widget)) != NULL) {
        return;
    }

    //TODO: delete everything and free it
}

/**
 * Render framebuffer with one program with arguments in shader_args. Each call increments 
 * con->processed_fbs_count by one, you have to reset it before calling this function.
 *
 * @param con RendererControl with framebuffers, vao and processed_fbs_count
 * @param preview Use preview framebuffers when rendering
 * @param program Program to use when rendering framebuffer
 * @param shader_args Array of ShaderArgument that will be used as program arguments
 * @param shader_args_count Count of ShaderArguments in shader_args
 * 
 * @return  void
 */
void render_fb(RendererControl* con, gboolean preview, GLuint program, ShaderArgument shader_args[], 
        int shader_args_count) {
    // Set output framebuffer
    if (preview) {
        glBindFramebuffer(GL_FRAMEBUFFER, con->processed_fbs_count % 2 ? 
            con->preview_fb2 : con->preview_fb1);
    } else {
        glBindFramebuffer(GL_FRAMEBUFFER, con->processed_fbs_count % 2 ? 
            con->fb2 : con->fb1);
    }

    // It's first processing, we have to use tex_base as source
    if (con->processed_fbs_count == 0) {
        glBindTexture(GL_TEXTURE_2D, con->tex_base);
    } else {
        // Set texture from other fb as input
        if (preview) {
            glBindTexture(GL_TEXTURE_2D, con->processed_fbs_count % 2 ? 
                con->preview_tex_fb1 : con->preview_tex_fb2);
        } else {
            glBindTexture(GL_TEXTURE_2D, con->processed_fbs_count % 2 ? 
                con->tex_fb1 : con->tex_fb2);
        }
    }

    // Prepare program
    glUseProgram(program);
    for (int i = 0; i < shader_args_count; i++) {
        glUniform1f(glGetUniformLocation(program, shader_args[i].name), shader_args[i].value);
    }
    // Render
    glBindVertexArray(con->VAO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

    con->processed_fbs_count++; // Increment for next framebuffer rendering
}

// NOTE: It seems it is not possible to use GL calls and/or use openGL outside GTK callbacks for 
// GLArea. Not following this results in undefined random behavior such as exporting wrong FB or
// straightup segfaulting with glReadPixels. Because of this we have to queue exports and any other
// potential OGL calls and do them on the next render() call
gboolean render(GtkGLArea* area, GdkGLContext* context, RendererControl* con) {
    if (gtk_gl_area_get_error (area) != NULL)
        return FALSE;

    if (con->image_data == NULL)
        return FALSE;

    if (con->texture_refresh_pending) {
        g_print("New image data ready to be shown, switching textures\n");
        refresh_textures(con);
        con->texture_refresh_pending = false;
    }

    if (con->export_pending) {
        g_print("Export pending, exporting to %s\n", con->export_path);

        export(con, con->export_path);
        free(con->export_path);
        con->export_path = NULL;
        con->export_pending = false;
    }

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

    glViewport(0, 0, con->preview_width, con->preview_height);
    //render_denoise_fb(con->preview_fb2, con->VAO, con->tex_base, con->programs.denoise, 5.0, con->settings.noise_reduction+0.001, con->settings.noise_reduction_sharpen+0.001);
    
    // TODO: I do not particularly like having to reset it before first render_fb call as it's
    // not obvious for the function caller to do so
    con->processed_fbs_count = 0; // We have to reset processed FrameBuffers count before processing
    render_fb(con, true, con->programs.denoise, (ShaderArgument[]){
        {"sigma", 5},
        {"kSigma", con->settings.noise_reduction+0.001},
        {"threshold", con->settings.noise_reduction_sharpen+0.001}
    }, 3);

    render_fb(con, true, con->programs.exposure, &(ShaderArgument){"value", con->settings.exposure}, 1);
    render_fb(con, true, con->programs.brightness, &(ShaderArgument){"value", con->settings.brightness}, 1);
    render_fb(con, true, con->programs.contrast, &(ShaderArgument){"value", con->settings.contrast}, 1);
    render_fb(con, true, con->programs.highlights, &(ShaderArgument){"value", con->settings.highlights}, 1);
    render_fb(con, true, con->programs.shadows, &(ShaderArgument){"value", con->settings.shadows}, 1);

    render_fb(con, true, con->programs.color_lightness, (ShaderArgument[]){
        {"value", con->settings.color_presets[0].color_lightness}, {"hue", 0}}, 2);
    render_fb(con, true, con->programs.color_lightness, (ShaderArgument[]){
        {"value", con->settings.color_presets[1].color_lightness}, {"hue", 120}}, 2);
    render_fb(con, true, con->programs.color_lightness, (ShaderArgument[]){
        {"value", con->settings.color_presets[2].color_lightness},{"hue", 240}}, 2);

    render_fb(con, true, con->programs.color_saturation, (ShaderArgument[]){
        {"value", con->settings.color_presets[0].color_saturation}, {"hue", 0}}, 2);
    render_fb(con, true, con->programs.color_saturation, (ShaderArgument[]){
        {"value", con->settings.color_presets[1].color_saturation}, {"hue", 120}}, 2);
    render_fb(con, true, con->programs.color_saturation, (ShaderArgument[]){
        {"value", con->settings.color_presets[2].color_saturation},{"hue", 240}}, 2);

    render_fb(con, true, con->programs.color_hue, (ShaderArgument[]){
        {"value", con->settings.color_presets[0].color_hue}, {"hue", 0}}, 2);
    render_fb(con, true, con->programs.color_lightness, (ShaderArgument[]){
        {"value", con->settings.color_presets[1].color_hue}, {"hue", 120}}, 2);
    render_fb(con, true, con->programs.color_lightness, (ShaderArgument[]){
        {"value", con->settings.color_presets[2].color_hue},{"hue", 240}}, 2);

    render_fb(con, true, con->programs.temperature, &(ShaderArgument){"value", con->settings.temperature}, 1);
    render_fb(con, true, con->programs.tint, &(ShaderArgument){"value", con->settings.tint}, 1);
    render_fb(con, true, con->programs.saturation, &(ShaderArgument){"value", con->settings.saturation}, 1);

    // Prepare GTK FB, set FB 2's texture as input and set rendering dimensions based on widget size
    gtk_gl_area_attach_buffers(area);

    if (con->show_original) {
        glBindTexture(GL_TEXTURE_2D, con->tex_base);
    } else {
        glBindTexture(GL_TEXTURE_2D, con->processed_fbs_count % 2 ? 
                con->preview_tex_fb1 : con->preview_tex_fb2);
    }
    
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

/**
 * Export image by rendering everything in full resolution, reading resulting pixels and saving them
 * to file in a specific format
 *
 * @param con Complete RendererControl struct
 * @param path Path where to save the exported image. This function takes ownership and 
 * handles freeing the resources
 *
 * @return True on success
 */
gboolean export(RendererControl* con, char* path) {
    int width = con->width;
    int height = con->height;

    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glViewport(0, 0, width, height);

    con->processed_fbs_count = 0; // We have to reset processed FrameBuffers count before processing
    render_fb(con, false, con->programs.denoise, (ShaderArgument[]){
        {"sigma", 5},
        {"kSigma", con->settings.noise_reduction+0.001},
        {"threshold", con->settings.noise_reduction_sharpen+0.001}
    }, 3);

    render_fb(con, false, con->programs.exposure, &(ShaderArgument){"value", con->settings.exposure}, 1);
    render_fb(con, false, con->programs.brightness, &(ShaderArgument){"value", con->settings.brightness}, 1);
    render_fb(con, false, con->programs.contrast, &(ShaderArgument){"value", con->settings.contrast}, 1);
    render_fb(con, false, con->programs.highlights, &(ShaderArgument){"value", con->settings.highlights}, 1);
    render_fb(con, false, con->programs.shadows, &(ShaderArgument){"value", con->settings.shadows}, 1);

    render_fb(con, false, con->programs.color_lightness, (ShaderArgument[]){
        {"value", con->settings.color_presets[0].color_lightness}, {"hue", 0}}, 2);
    render_fb(con, false, con->programs.color_lightness, (ShaderArgument[]){
        {"value", con->settings.color_presets[1].color_lightness}, {"hue", 120}}, 2);
    render_fb(con, false, con->programs.color_lightness, (ShaderArgument[]){
        {"value", con->settings.color_presets[2].color_lightness},{"hue", 240}}, 2);

    render_fb(con, false, con->programs.color_saturation, (ShaderArgument[]){
        {"value", con->settings.color_presets[0].color_saturation}, {"hue", 0}}, 2);
    render_fb(con, false, con->programs.color_saturation, (ShaderArgument[]){
        {"value", con->settings.color_presets[1].color_saturation}, {"hue", 120}}, 2);
    render_fb(con, false, con->programs.color_saturation, (ShaderArgument[]){
        {"value", con->settings.color_presets[2].color_saturation},{"hue", 240}}, 2);

    render_fb(con, false, con->programs.color_hue, (ShaderArgument[]){
        {"value", con->settings.color_presets[0].color_hue}, {"hue", 0}}, 2);
    render_fb(con, false, con->programs.color_lightness, (ShaderArgument[]){
        {"value", con->settings.color_presets[1].color_hue}, {"hue", 120}}, 2);
    render_fb(con, false, con->programs.color_lightness, (ShaderArgument[]){
        {"value", con->settings.color_presets[2].color_hue},{"hue", 240}}, 2);

    render_fb(con, false, con->programs.temperature, &(ShaderArgument){"value", con->settings.temperature}, 1);
    render_fb(con, false, con->programs.tint, &(ShaderArgument){"value", con->settings.tint}, 1);
    render_fb(con, false, con->programs.saturation, &(ShaderArgument){"value", con->settings.saturation}, 1);
    
    uint8_t* fb_data = calloc(width*height, sizeof(uint8_t)*3);

    glReadnPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, sizeof(uint8_t)*3*width*height, fb_data);

    // Flush the contents of the pipeline
    glFlush();

    GdkPixbuf* pxbuf = gdk_pixbuf_new_from_data(fb_data, GDK_COLORSPACE_RGB, false, 8, width, height, width*3, NULL, NULL);

    gdk_pixbuf_save(pxbuf, path, "jpeg", NULL, "quality", "100", NULL);

    free(path);

    return true;
}

void redraw_image(GtkGLArea* gl_area) {
    gtk_gl_area_queue_render(gl_area);
}

