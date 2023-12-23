#include "hw-renderer.h"
#include <stdio.h>
#include <string.h>


int hw_init(void) {
    return 0;
}

void hw_set(char* function_name, uint8_t *pixels, int width, int height, double val) {
    hw_set_params(function_name, pixels, width, height, val, NULL, 0);
}

void hw_set_params(char* function_name, uint8_t *pixels, int width, int height, double val,
                    int params[], int params_count) {
    return;
}

