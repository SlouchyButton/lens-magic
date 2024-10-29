#pragma once

#define CL_TARGET_OPENCL_VERSION 300

#include <CL/opencl.h>

typedef struct RendererControl_ RendererControl;

typedef struct CLPrograms_ {
    cl_kernel  exposure;
    cl_kernel  brightness;
    cl_kernel  contrast;
    cl_kernel  highlights;
    cl_kernel  shadows;
    cl_kernel  temperature;
    cl_kernel  tint;
    cl_kernel  saturation;
    cl_kernel  color_hue;
    cl_kernel  color_saturation;
    cl_kernel  color_lightness;
    cl_kernel  denoise;
    cl_kernel  bitdepth;
    cl_kernel  scale;
} CLPrograms;

typedef struct CLInstance_ {
    cl_context          context;
    cl_command_queue    command_queue;
    CLPrograms          programs;
    cl_device_id        device_id;
    cl_mem              input;
    cl_mem              memA;
    cl_mem              memB;
    cl_mem              preview_input;
    cl_mem              preview_memA;
    cl_mem              preview_memB;
} CLInstance;

typedef struct KernelArgument_ {
    const char* name;
    double      value;
} KernelArgument;

int cl_init(CLInstance* cl_instance);
int cl_render(RendererControl* con);