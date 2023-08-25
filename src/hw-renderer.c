#include "hw-renderer.h"
#include <stdio.h>

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

void prepare_buffers(uint8_t *data, int len) {
    cl_int err = 0;
    input = clCreateBuffer(context,  CL_MEM_READ_ONLY,  len, NULL, &err);
    output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, len, NULL, &err);

    err = clEnqueueWriteBuffer(commands, input, CL_TRUE, 0, len, data, 0, NULL, NULL);

    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
}

void hw_set_exposure(uint8_t *pixels, int pixel_size, int len, double val) {
    cl_int err = 0;
    kernel = clCreateKernel(program, "exposure", &err);

    prepare_buffers(pixels, len);

    err = clSetKernelArg(kernel, 2, sizeof(double), &val);

    size_t size = len;
    size_t chunk = pixel_size;
    err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &size, &chunk, 0, NULL, NULL);

    clFinish(commands);

    err = clEnqueueReadBuffer( commands, output, CL_TRUE, 0, sizeof(uint8_t) * len, pixels, 0, NULL, NULL );

    clReleaseMemObject(input);
    clReleaseMemObject(output);
}

void hw_set_brightness(uint8_t *pixels, int pixel_size, int len, double val) {
    cl_int err = 0;
    kernel = clCreateKernel(program, "brightness", &err);

    prepare_buffers(pixels, len);

    err = clSetKernelArg(kernel, 2, sizeof(double), &val);

    size_t size = len;
    size_t chunk = pixel_size;
    err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &size, &chunk, 0, NULL, NULL);

    clFinish(commands);

    err = clEnqueueReadBuffer( commands, output, CL_TRUE, 0, sizeof(uint8_t) * len, pixels, 0, NULL, NULL );

    clReleaseMemObject(input);
    clReleaseMemObject(output);
}

void hw_set_contrast(uint8_t *pixels, int pixel_size, int len, double val) {
    cl_int err = 0;
    kernel = clCreateKernel(program, "contrast", &err);

    prepare_buffers(pixels, len);

    err = clSetKernelArg(kernel, 2, sizeof(double), &val);

    size_t size = len;
    size_t chunk = pixel_size;
    err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &size, &chunk, 0, NULL, NULL);

    clFinish(commands);

    err = clEnqueueReadBuffer( commands, output, CL_TRUE, 0, sizeof(uint8_t) * len, pixels, 0, NULL, NULL );

    clReleaseMemObject(input);
    clReleaseMemObject(output);
}

void hw_set_saturation(uint8_t *pixels, int pixel_size, int len, double val) {
    cl_int err = 0;
    kernel = clCreateKernel(program, "saturation", &err);

    prepare_buffers(pixels, len);

    err = clSetKernelArg(kernel, 2, sizeof(double), &val);

    size_t size = len;
    size_t chunk = pixel_size;
    err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &size, &chunk, 0, NULL, NULL);

    clFinish(commands);

    err = clEnqueueReadBuffer( commands, output, CL_TRUE, 0, sizeof(uint8_t) * len, pixels, 0, NULL, NULL );

    clReleaseMemObject(input);
    clReleaseMemObject(output);
}

