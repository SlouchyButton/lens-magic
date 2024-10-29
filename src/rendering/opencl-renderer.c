#include <stdio.h>

#include "opencl-renderer.h"
#include "kernels/kernels-source.h"

#include "renderer.h"

cl_kernel create_kernel(CLInstance* cl_instance, const char **kernel_source) {
    int err = 0;
    cl_program program = clCreateProgramWithSource(cl_instance->context,
                                                    1, kernel_source,
                                                    NULL, &err);
    if (!program || err != CL_SUCCESS)
    {
        printf("Failed to create program #%d\n", err);
        return NULL;
    }

    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        char buffer[2048];

        printf("Failed to build program executable\n");
        clGetProgramBuildInfo(program, cl_instance->device_id,
                                CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, NULL);
        printf("%s\n", buffer);
        return NULL;
    }
    cl_kernel kernel = clCreateKernel(program, "entry", &err);
    if (!kernel || err != CL_SUCCESS)
    {
        printf("Failed to create kernel #%d\n", err);
        return NULL;
    }
    return kernel;
}

void prepare_programs(CLInstance* cl_instance) {
    printf("Building exposure\n");
    cl_instance->programs.exposure = create_kernel(cl_instance, &exposure);
    /*cl_instance->programs.brightness = create_program(vertex_shader, brightness_shader);
    cl_instance->programs.contrast = create_program(vertex_shader, contrast_shader);
    cl_instance->programs.temperature = create_program(vertex_shader, temperature_shader);
    cl_instance->programs.tint = create_program(vertex_shader, tint_shader);
    cl_instance->programs.saturation = create_program(vertex_shader, saturation_shader);
    cl_instance->programs.highlights = create_program(vertex_shader, highlights_shader);
    cl_instance->programs.shadows = create_program(vertex_shader, shadows_shader);
    cl_instance->programs.denoise = create_program(vertex_shader, denoie_shader);

    cl_instance->programs.color_hue = create_program(vertex_shader, color_hue_shader);
    cl_instance->programs.color_saturation = create_program(vertex_shader, color_saturation_shader);
    cl_instance->programs.color_lightness = create_program(vertex_shader, color_lightness_shader);*/
    printf("Building bitdepth\n");
    cl_instance->programs.bitdepth = create_kernel(cl_instance, &bitdepth);
    printf("Building scale\n");
    cl_instance->programs.scale = create_kernel(cl_instance, &scale);
    printf("All programs built\n");
}

int cl_init(CLInstance* cl_instance) {
    memset(cl_instance, 0, sizeof(CLInstance));

    cl_platform_id platforms[15];
    cl_uint platform_cnt;

    int err = clGetPlatformIDs(15, platforms, &platform_cnt);
    if (err != CL_SUCCESS)
    {
        printf("Failed to detect platforms #%d\n", err);
        return 1;
    }
    printf("Detected %d platforms:\n", platform_cnt);

    char name[50] = {0};
    for (int i = 0; i < platform_cnt; i++) {
        clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 49, name, NULL);
        printf("%s\n", name);
    }

    cl_device_id device_ids[15];
    cl_uint device_cnt;
    err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_DEFAULT, 15, device_ids,
                            &device_cnt);
    if (err != CL_SUCCESS)
    {
        printf("Failed to get device ID #%d\n", err);
        return 1;
    }
    printf("Detected %d devices:\n", device_cnt);
    for (int i = 0; i < device_cnt; i++) {
        clGetDeviceInfo(device_ids[i], CL_DEVICE_NAME, 49, name, NULL);
        printf("%s\n", name);
    }
    clGetDeviceInfo(device_ids[0], CL_DEVICE_NAME, 49, name, NULL);
    printf("Choosing OCL device: %s\n", name);
    cl_instance->device_id = device_ids[0];

    cl_instance->context = clCreateContext(NULL, 1, &cl_instance->device_id,
                                            NULL, NULL, &err);
    if (!cl_instance->context || err != CL_SUCCESS)
    {
        printf("Failed to create a compute context\n");
        return 1;
    }

    cl_instance->command_queue = clCreateCommandQueueWithProperties(
                                    cl_instance->context, cl_instance->device_id,
                                    NULL, &err
                                );
    if (!cl_instance->command_queue || err != CL_SUCCESS)
    {
        printf("Failed to create a command queue\n");
        return 1;
    }

    prepare_programs(cl_instance);

    return 0;
}

int cl_render_enqueue(RendererControl* con, gboolean preview, cl_kernel kernel,
        KernelArgument kernel_args[], int kernel_args_count) {

    // It's first processing, we have to use input as source
    cl_mem input = NULL;
    if (con->processed_fbs_count == 0) {
        input = con->cl_instance.preview_input;
    } else {
        // Set texture from other fb as input
        if (preview || 1) {
            input = con->processed_fbs_count % 2 ? 
                con->cl_instance.preview_memA : con->cl_instance.preview_memB;
        } else {
            //glBindTexture(GL_TEXTURE_2D, con->processed_fbs_count % 2 ? 
            //    con->tex_fb1 : con->tex_fb2);
        }
    }

    // Set output framebuffer
    cl_mem output = NULL;
    if (preview || 1) {
        output = con->processed_fbs_count % 2 ? 
            con->cl_instance.preview_memB : con->cl_instance.preview_memA;
    } else {
        //glBindFramebuffer(GL_FRAMEBUFFER, con->processed_fbs_count % 2 ? 
        //    con->fb2 : con->fb1);
    }

    int err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
    if (err != CL_SUCCESS)
    {
        printf("Failed to set input buffer as a kernel argument (%d)\n", err);
        return 1;
    }
    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
    if (err != CL_SUCCESS)
    {
        printf("Failed to set output buffer as a kernel argument (%d)\n", err);
        return 1;
    }

    for (int i = 0; i < kernel_args_count; i++) {
        err = clSetKernelArg(kernel, i+2, sizeof(double), &kernel_args[i].value);
        if (err != CL_SUCCESS)
        {
            printf("Failed to set param %s as a kernel argument (%d)\n", kernel_args[i].name, err);
            return 1;
        }
    }

    err = clEnqueueNDRangeKernel(con->cl_instance.command_queue, kernel, 2, NULL,
                                (size_t[2]){con->preview_width, con->preview_height}, NULL, 0,
                                NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Failed to enqueue work (%d)\n", err);
        return 1;
    }

    con->processed_fbs_count++; // Increment for next framebuffer rendering
    return 0;
}

int cl_render(RendererControl* con){
    cl_image_format format;
    format.image_channel_order = CL_RGBA;
    format.image_channel_data_type = con->bit_depth == 16 ? CL_UNORM_INT16 : CL_UNORM_INT8;
    
    cl_image_format format_t = {0};
    format_t.image_channel_order = CL_RGBA;
    format_t.image_channel_data_type = CL_UNSIGNED_INT8;

    cl_image_desc desc;
    desc.image_type = CL_MEM_OBJECT_IMAGE2D;
    desc.image_width = con->width;
    desc.image_height = con->height;
    desc.image_depth = 0;
    desc.image_array_size = 0;
    desc.image_row_pitch = 0;
    desc.image_slice_pitch = 0;
    desc.num_mip_levels = 0;
    desc.num_samples = 0;
    desc.buffer = NULL;

    cl_image_desc desc_t;
    desc_t.image_type = CL_MEM_OBJECT_IMAGE2D;
    desc_t.image_width = con->preview_width;
    desc_t.image_height = con->preview_height;
    desc_t.image_depth = 0;
    desc_t.image_array_size = 0;
    desc_t.image_row_pitch = 0;
    desc_t.image_slice_pitch = 0;
    desc_t.num_mip_levels = 0;
    desc_t.num_samples = 0;
    desc_t.buffer = NULL;

    int err = 0;
    con->cl_instance.input = clCreateImage(con->cl_instance.context,
                                            CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                            &format, &desc, con->image_data, &err);
    if (err != CL_SUCCESS)
    {
        printf("Failed to create input buffer (%d)\n", err);
        return 1;
    }

    con->cl_instance.preview_input = clCreateImage(con->cl_instance.context,
                                            CL_MEM_READ_WRITE, &format, &desc_t,
                                            NULL, &err);
    if (err != CL_SUCCESS)
    {
        printf("Failed to create A buffer (%d)\n", err);
        return 1;
    }

    con->cl_instance.preview_memA = clCreateImage(con->cl_instance.context,
                                            CL_MEM_READ_WRITE, &format, &desc_t,
                                            NULL, &err);
    if (err != CL_SUCCESS)
    {
        printf("Failed to create A buffer (%d)\n", err);
        return 1;
    }

    con->cl_instance.preview_memB = clCreateImage(con->cl_instance.context,
                                            CL_MEM_READ_WRITE, &format, &desc_t,
                                            NULL, &err);
    if (err != CL_SUCCESS)
    {
        printf("Failed to create B buffer (%d)\n", err);
        return 1;
    }

    cl_mem out_mem = clCreateImage(con->cl_instance.context,
                                            CL_MEM_WRITE_ONLY, &format_t, &desc_t,
                                            NULL, &err);
    if (err != CL_SUCCESS)
    {
        printf("Failed to create out buffer (%d)\n", err);
        return 1;
    }

    // TODO: I do not particularly like having to reset it before first cl_render_enqueue call as it's
    // not obvious for the function caller to do so
    con->processed_fbs_count = 0; // We have to reset processed FrameBuffers count before processing
    
    /*cl_render_enqueue(con, true, con->cl_instance.programs.denoise, (KernelArgument[]){
        {"sigma", 5},
        {"kSigma", con->settings.noise_reduction+0.001},
        {"threshold", con->settings.noise_reduction_sharpen+0.001}
    }, 3);*/

    err = clSetKernelArg(con->cl_instance.programs.scale, 0, sizeof(cl_mem), &con->cl_instance.input);
    if (err != CL_SUCCESS)
    {
        printf("SCALE: Failed to set input buffer as a kernel argument (%d)\n", err);
        return 1;
    }
    err = clSetKernelArg(con->cl_instance.programs.scale, 1, sizeof(cl_mem), &con->cl_instance.preview_input);
    if (err != CL_SUCCESS)
    {
        printf("SCALE: Failed to set output buffer as a kernel argument (%d)\n", err);
        return 1;
    }

    err = clSetKernelArg(con->cl_instance.programs.scale, 2, sizeof(int), &con->width);
    err |= clSetKernelArg(con->cl_instance.programs.scale, 3, sizeof(int), &con->height);
    err |= clSetKernelArg(con->cl_instance.programs.scale, 4, sizeof(int), &con->preview_width);
    err |= clSetKernelArg(con->cl_instance.programs.scale, 5, sizeof(int), &con->preview_height);
    if (err != CL_SUCCESS)
    {
        printf("Failed to set width/heigh as a kernel argument\n");
        return 1;
    }

    err = clEnqueueNDRangeKernel(con->cl_instance.command_queue, con->cl_instance.programs.scale, 2, NULL,
                                (size_t[2]){con->width, con->height}, NULL, 0,
                                NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Failed to enqueue work (%d)\n", err);
        return 1;
    }

    cl_render_enqueue(con, true, con->cl_instance.programs.exposure, &(KernelArgument){"value", con->settings.exposure}, 1);
    /*cl_render_enqueue(con, true, con->cl_instance.programs.brightness, &(KernelArgument){"value", con->settings.brightness}, 1);
    cl_render_enqueue(con, true, con->cl_instance.programs.contrast, &(KernelArgument){"value", con->settings.contrast}, 1);
    cl_render_enqueue(con, true, con->cl_instance.programs.highlights, &(KernelArgument){"value", con->settings.highlights}, 1);
    cl_render_enqueue(con, true, con->cl_instance.programs.shadows, &(KernelArgument){"value", con->settings.shadows}, 1);

    cl_render_enqueue(con, true, con->cl_instance.programs.color_lightness, (KernelArgument[]){
        {"value", con->settings.color_presets[0].color_lightness}, {"hue", 0}}, 2);
    cl_render_enqueue(con, true, con->cl_instance.programs.color_lightness, (KernelArgument[]){
        {"value", con->settings.color_presets[1].color_lightness}, {"hue", 120}}, 2);
    cl_render_enqueue(con, true, con->cl_instance.programs.color_lightness, (KernelArgument[]){
        {"value", con->settings.color_presets[2].color_lightness},{"hue", 240}}, 2);

    cl_render_enqueue(con, true, con->cl_instance.programs.color_saturation, (KernelArgument[]){
        {"value", con->settings.color_presets[0].color_saturation}, {"hue", 0}}, 2);
    cl_render_enqueue(con, true, con->cl_instance.programs.color_saturation, (KernelArgument[]){
        {"value", con->settings.color_presets[1].color_saturation}, {"hue", 120}}, 2);
    cl_render_enqueue(con, true, con->cl_instance.programs.color_saturation, (KernelArgument[]){
        {"value", con->settings.color_presets[2].color_saturation},{"hue", 240}}, 2);

    cl_render_enqueue(con, true, con->cl_instance.programs.color_hue, (KernelArgument[]){
        {"value", con->settings.color_presets[0].color_hue}, {"hue", 0}}, 2);
    cl_render_enqueue(con, true, con->cl_instance.programs.color_hue, (KernelArgument[]){
        {"value", con->settings.color_presets[1].color_hue}, {"hue", 120}}, 2);
    cl_render_enqueue(con, true, con->cl_instance.programs.color_hue, (KernelArgument[]){
        {"value", con->settings.color_presets[2].color_hue},{"hue", 240}}, 2);

    cl_render_enqueue(con, true, con->cl_instance.programs.temperature, &(KernelArgument){"value", con->settings.temperature}, 1);
    cl_render_enqueue(con, true, con->cl_instance.programs.tint, &(KernelArgument){"value", con->settings.tint}, 1);
    cl_render_enqueue(con, true, con->cl_instance.programs.saturation, &(KernelArgument){"value", con->settings.saturation}, 1);*/

    err = clSetKernelArg(con->cl_instance.programs.bitdepth, 0, sizeof(cl_mem), con->processed_fbs_count % 2 ? 
                                &con->cl_instance.preview_memA : &con->cl_instance.preview_memB);
    if (err != CL_SUCCESS)
    {
        printf("BITDEPTH: Failed to set input buffer as a kernel argument (%d)\n", err);
        return 1;
    }
    err = clSetKernelArg(con->cl_instance.programs.bitdepth, 1, sizeof(cl_mem), &out_mem);
    if (err != CL_SUCCESS)
    {
        printf("BITDEPTH: Failed to set output buffer as a kernel argument (%d)\n", err);
        return 1;
    }

    err = clEnqueueNDRangeKernel(con->cl_instance.command_queue, con->cl_instance.programs.bitdepth, 2, NULL,
                                (size_t[2]){con->preview_width, con->preview_height}, NULL, 0,
                                NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Failed to enqueue work (%d)\n", err);
        return 1;
    }

    clFinish(con->cl_instance.command_queue);

    free(con->preview_image_data);
    con->preview_image_data = malloc(con->preview_width * con->preview_height * 4);

    err = clEnqueueReadImage(con->cl_instance.command_queue, out_mem, CL_TRUE, (size_t[3]){0, 0, 0},
                                (size_t[3]){con->preview_width, con->preview_height, 1}, 0, 0, con->preview_image_data, 0, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Failed to enqueue work (%d)\n", err);
        return 1;
    }

    clReleaseMemObject(con->cl_instance.input);
    clReleaseMemObject(con->cl_instance.preview_input);
    clReleaseMemObject(con->cl_instance.preview_memA);
    clReleaseMemObject(con->cl_instance.preview_memB);
    clReleaseMemObject(out_mem);

    return 0;
}