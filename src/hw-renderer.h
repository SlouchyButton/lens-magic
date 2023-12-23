#pragma once

#define CL_TARGET_OPENCL_VERSION 300

#include <inttypes.h>

int hw_init(void);
void hw_set(char* function_name, uint8_t *pixels, int width, int height, double val);
void hw_set_params(char* function_name, uint8_t *pixels, int width, int height, double val,
                    int params[], int params_count);
