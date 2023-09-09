#include "hw-renderer.h"
#include <stdio.h>
#include <string.h>

#include "kernel-source.h"

cl_platform_id platforms[5];
cl_uint platform_cnt;

cl_device_id device_id;             // compute device id
cl_context context;                 // compute context
cl_command_queue commands;          // compute command queue
cl_program program;                 // compute program
cl_kernel kernel;                   // compute kernel

cl_mem input;                       // device memory used for the input array
cl_mem output;                      // device memory used for the output array
char name[50] = {0};

int hw_init(void) {
    int err = clGetPlatformIDs(5, platforms, &platform_cnt);
    printf("Detected %d platforms:\n", platform_cnt);

    for (int i = 0; i < platform_cnt; i++) {
        clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 49, name, NULL);
        printf("%s\n", name);
    }

    err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_DEFAULT, 1, &device_id, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Failed to get device ID #%d\n", err);
        return 1;
    }
    clGetDeviceInfo(device_id, CL_DEVICE_NAME, 49, name, NULL);
    printf("Chosen OCL device: %s\n", name);

    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);
    if (!context || err != CL_SUCCESS)
    {
        printf("Failed to create a compute context\n");
        return 1;
    }

    commands = clCreateCommandQueueWithProperties(context, device_id, NULL, &err);
    if (!commands || err != CL_SUCCESS)
    {
        printf("Failed to create a command queue\n");
        return 1;
    }

    program = clCreateProgramWithSource(context, 1, (const char **) & KernelSource, NULL, &err);
    if (!program || err != CL_SUCCESS)
    {
        printf("Failed to create compute program\n");
        return 1;
    }

    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        char buffer[2048];

        printf("Failed to build program executable\n");
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, NULL);
        printf("%s\n", buffer);
        return 1;
    }

    // Shutdown and cleanup
    //
    /*clReleaseMemObject(input);
    clReleaseMemObject(output);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);*/
    return 0;
}

void hw_set(char* function_name, uint8_t *pixels, int width, int height, double val) {
    hw_set_params(function_name, pixels, width, height, val, NULL, 0);
}

void hw_set_params(char* function_name, uint8_t *pixels, int width, int height, double val,
                    int params[], int params_count) {
    int len = width*height;
    cl_int err = 0;
    kernel = clCreateKernel(program, function_name, &err);

    cl_image_format format;
    format.image_channel_order = CL_RGBA;
    format.image_channel_data_type = CL_UNSIGNED_INT8;

    cl_image_desc desc;
    desc.image_type = CL_MEM_OBJECT_IMAGE2D;
    desc.image_width = width;
    desc.image_height = height;
    desc.image_depth = 0;
    desc.image_array_size = 0;
    desc.image_row_pitch = 0;
    desc.image_slice_pitch = 0;
    desc.num_mip_levels = 0;
    desc.num_samples = 0;
    desc.buffer = NULL;

    //input = clCreateBuffer(context,  CL_MEM_READ_ONLY,  len, NULL, &err);
    input = clCreateImage(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, &format, &desc, pixels, &err);
    if (err != CL_SUCCESS)
    {
        printf("Failed to create input buffer (%d)\n", err);
        return;
    }

    output = clCreateImage(context, CL_MEM_WRITE_ONLY, &format, &desc, NULL, &err);
    if (err != CL_SUCCESS)
    {
        printf("Failed to create output buffer (%d)\n", err);
        return;
    }

    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
    if (err != CL_SUCCESS)
    {
        printf("Failed to set input buffer as a kernel argument (%d)\n", err);
        return;
    }
    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
    if (err != CL_SUCCESS)
    {
        printf("Failed to set output buffer as a kernel argument (%d)\n", err);
        return;
    }

    err = clSetKernelArg(kernel, 2, sizeof(double), &val);
    if (err != CL_SUCCESS)
    {
        printf("Failed to set value as a kernel argument (%d)\n", err);
        return;
    }

    for (int i = 0; i < params_count; i++) {
        err = clSetKernelArg(kernel, i+3, sizeof(int), &params[i]);
        if (err != CL_SUCCESS)
        {
            printf("Failed to set param #%d as a kernel argument (%d)\n", i, err);
            return;
        }
    }

    err = clEnqueueNDRangeKernel(commands, kernel, 2, NULL, (size_t[2]){width, height}, NULL, 0, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Failed to enqueue work (%d)\n", err);
        return;
    }

    clFinish(commands);

    err = clEnqueueReadImage(commands, output, CL_TRUE, (size_t[3]){0, 0, 0},
                                (size_t[3]){width, height, 1}, 0, 0, pixels, 0, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Failed to enqueue work (%d)\n", err);
        return;
    }

    clReleaseMemObject(input);
    clReleaseMemObject(output);
}

