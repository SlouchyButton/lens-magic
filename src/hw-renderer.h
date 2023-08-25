#pragma once

#define CL_TARGET_OPENCL_VERSION 300

#include <CL/opencl.h>

int hw_init(void);
void hw_set_exposure(uint8_t *pixels, int pixel_size, int len, double val);
void hw_set_brightness(uint8_t *pixels, int pixel_size, int len, double val);
void hw_set_contrast(uint8_t *pixels, int pixel_size, int len, double val);
void hw_set_saturation(uint8_t *pixels, int pixel_size, int len, double val);
