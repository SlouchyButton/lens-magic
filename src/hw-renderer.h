#pragma once

#define CL_TARGET_OPENCL_VERSION 300

#include <CL/opencl.h>

int hw_init(void);
void hw_set(char* function_name, uint8_t *pixels, int pixel_size, int len, double val);

