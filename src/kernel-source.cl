__kernel void rgb_to_hsl(unsigned int r, unsigned int g, unsigned int b,
                            __local double *h, __local double *s, __local double *l) {
    double dr = r / 255.0;
    double dg = g / 255.0;
    double db = b / 255.0;

    double max_val = max(dr, max(dg, db));
    double min_val = min(dr, min(dg, db));
    double delta = max_val - min_val;

    if (delta == 0) {
        *h = 0;
        *s = 0;
        *l = dr;
        return;
    }

    if (max_val == dr) {
        *h = 60 * fmod(((dg-db)/delta), 6);
    }
    if (max_val == dg) {
        *h = 60 * ((db-dr)/delta + 2);
    }
    if (max_val == db) {
        *h = 60 * ((dr-dg)/delta + 4);
    }

    if (*h < 0) {
        *h = 360+*h;
    }

    *l = (max_val + min_val) / 2;
    *s = delta / (1 - fabs(2*(*l) - 1));
}

__kernel void hsl_to_rgb(double h, double s, double l,
                __local unsigned int* r, __local unsigned int* g, __local unsigned int* b) {
    double c = s * (1 - fabs(2*l-1));
    double x = c * (1 - fabs(fmod(h/60, 2) - 1));
    double m = l - c/2;

    double r_temp = 0;
    double g_temp = 0;
    double b_temp = 0;

    if (h >= 0 && h < 60) {
        r_temp = c;
        g_temp = x;
    }
    if (h >= 60 && h < 120) {
        r_temp = x;
        g_temp = c;
    }
    if (h >= 120 && h < 180) {
        g_temp = c;
        b_temp = x;
    }
    if (h >= 180 && h < 240) {
        g_temp = x;
        b_temp = c;
    }
    if (h >= 240 && h < 300) {
        r_temp = x;
        b_temp = c;
    }
    if (h >= 300 && h < 360) {
        r_temp = c;
        b_temp = x;
    }

    *r = round((r_temp+m)*255);
    *g = round((g_temp+m)*255);
    *b = round((b_temp+m)*255);
}

__kernel void exposure(__read_only image2d_t input, __write_only image2d_t output, const double val) {
    const int2 pos = (int2)(get_global_id(0), get_global_id(1));
    uint4 pixel = read_imageui(input, pos);

    for (int j = 0; j < 3; j++) {
        int new_val = pixel[j] * pow(2, val);
        if (new_val > 255) {
            pixel.x = 255;
            pixel.y = 255;
            pixel.z = 255;
            break;
        } else if (new_val < 0) {
            pixel.x = 0;
            pixel.y = 0;
            pixel.z = 0;
            break;
        } else {
            pixel[j] = new_val;
        }
    }
    write_imageui(output, pos, pixel);
}

__kernel void brightness(__read_only image2d_t input, __write_only image2d_t output, const double val) {
    const int2 pos = (int2)(get_global_id(0), get_global_id(1));
    uint4 pixel = read_imageui(input, pos);

    for (int j = 0; j < 3; j++) {
        int new_val = pixel[j] + val;
        if (new_val > 255) {
            pixel.x = 255;
            pixel.y = 255;
            pixel.z = 255;
            break;
        } else if (new_val < 0) {
            pixel.x = 0;
            pixel.y = 0;
            pixel.z = 0;
            break;
        } else {
            pixel[j] = new_val;
        }
    }
    write_imageui(output, pos, pixel);
}

__kernel void contrast(__read_only image2d_t input, __write_only image2d_t output, const double val) {
    const int2 pos = (int2)(get_global_id(0), get_global_id(1));
    uint4 pixel = read_imageui(input, pos);

    for (int j = 0; j < 3; j++) {
        int new_val = val*((int)pixel[j]-128)+128;
        if (new_val > 255) {
            pixel.x = 255;
            pixel.y = 255;
            pixel.z = 255;
            break;
        } else if (new_val < 0) {
            pixel.x = 0;
            pixel.y = 0;
            pixel.z = 0;
            break;
        } else {
            pixel[j] = new_val;
        }
    }
    write_imageui(output, pos, pixel);
}

__kernel void temperature(__read_only image2d_t input, __write_only image2d_t output, const double val) {
    const int2 pos = (int2)(get_global_id(0), get_global_id(1));
    uint4 pixel = read_imageui(input, pos);

    int newR = (int)pixel.x + val;
    if (newR > 255) {
        newR = 255;
    } else if (newR < 0) {
        newR = 0;
    }
    int newB = (int)pixel.z - val;
    if (newB > 255) {
        newB = 255;
    } else if (newB < 0) {
        newB = 0;
    }
    pixel.x = newR;
    pixel.z = newB;
    write_imageui(output, pos, pixel);
}

__kernel void tint(__read_only image2d_t input, __write_only image2d_t output, const double val) {
    const int2 pos = (int2)(get_global_id(0), get_global_id(1));
    uint4 pixel = read_imageui(input, pos);

    int newR = (int)pixel.x + val;
    if (newR > 255) {
        newR = 255;
    } else if (newR < 0) {
        newR = 0;
    }
    int newG = (int)pixel.y - val;
    if (newG > 255) {
        newG = 255;
    } else if (newG < 0) {
        newG = 0;
    }

    int newB = (int)pixel.z + val;
    if (newB > 255) {
        newB = 255;
    } else if (newB < 0) {
        newB = 0;
    }

    pixel.x = newR;
    pixel.y = newG;
    pixel.z = newB;
    write_imageui(output, pos, pixel);
}

__kernel void saturation(__read_only image2d_t input, __write_only image2d_t output, const double val) {
    const int2 pos = (int2)(get_global_id(0), get_global_id(1));
    uint4 pixel = read_imageui(input, pos);

    __local unsigned int r;
    __local unsigned int g;
    __local unsigned int b;

    r = pixel.x;
    g = pixel.y;
    b = pixel.z;

    __local double h;
    __local double s;
    __local double l;

    h = 0;
    s = 0;
    l = 0;

    rgb_to_hsl(r,g,b, &h,&s,&l);

    double modifier = s * val;

    if (s + modifier > 1) {
        s = 1;
    } else if (s + modifier < 0) {
        s = 0;
    } else {
        s += modifier;
    }

    hsl_to_rgb(h,s,l, &r,&g,&b);

    pixel.x = r;
    pixel.y = g;
    pixel.z = b;
    write_imageui(output, pos, pixel);
}

__kernel void highlights(__read_only image2d_t input, __write_only image2d_t output, const double val) {
    const int2 pos = (int2)(get_global_id(0), get_global_id(1));
    uint4 pixel = read_imageui(input, pos);

    __local unsigned int r;
    __local unsigned int g;
    __local unsigned int b;

    r = pixel.x;
    g = pixel.y;
    b = pixel.z;

    __local double h;
    __local double s;
    __local double l;

    h = 0;
    s = 0;
    l = 0;

    rgb_to_hsl(r,g,b, &h,&s,&l);

    double coefficient = (1 - l*2.5)*-1;
    if (coefficient < 0)
        coefficient = 0;

    double modifier = (val) * coefficient + 1;

    for (int j = 0; j < 3; j++) {
        int new_val = modifier*((int)pixel[j]-128)+128;
        if (new_val > 255) {
            pixel.x = 255;
            pixel.y = 255;
            pixel.z = 255;
            break;
        } else if (new_val < 0) {
            pixel.x = 0;
            pixel.y = 0;
            pixel.z = 0;
            break;
        } else {
            pixel[j] = new_val;
        }
    }
    write_imageui(output, pos, pixel);
}

__kernel void shadows(__read_only image2d_t input, __write_only image2d_t output, const double val) {
    const int2 pos = (int2)(get_global_id(0), get_global_id(1));
    uint4 pixel = read_imageui(input, pos);

    __local unsigned int r;
    __local unsigned int g;
    __local unsigned int b;

    r = pixel.x;
    g = pixel.y;
    b = pixel.z;

    __local double h;
    __local double s;
    __local double l;

    h = 0;
    s = 0;
    l = 0;

    rgb_to_hsl(r,g,b, &h,&s,&l);

    double coefficient = 1 - l*2.5;
    if (coefficient < 0)
        coefficient = 0;

    double modifier = (val * -1) * coefficient + 1;

    for (int j = 0; j < 3; j++) {
        int new_val = modifier*((int)pixel[j]-128)+128;
        if (new_val > 255) {
            pixel.x = 255;
            pixel.y = 255;
            pixel.z = 255;
            break;
        } else if (new_val < 0) {
            pixel.x = 0;
            pixel.y = 0;
            pixel.z = 0;
            break;
        } else {
            pixel[j] = new_val;
        }
    }
    write_imageui(output, pos, pixel);
}

__kernel void color_hue(__read_only image2d_t input, __write_only image2d_t output, const double val,
                        int hue_start, int hue_end) {
    const int2 pos = (int2)(get_global_id(0), get_global_id(1));
    uint4 pixel = read_imageui(input, pos);

    __local unsigned int r;
    __local unsigned int g;
    __local unsigned int b;

    r = pixel.x;
    g = pixel.y;
    b = pixel.z;

    __local double h;
    __local double s;
    __local double l;

    h = 0;
    s = 0;
    l = 0;

    rgb_to_hsl(r,g,b, &h,&s,&l);

    int coefficient = 1;
    if (h > hue_end && h < hue_start) {
        coefficient = 1 - max((h - hue_end), (hue_start - h)) / 10;
        if (coefficient <= 0) {
            write_imageui(output, pos, pixel);
            return;
        }
    }

    if (h - val * coefficient < 0) {
        h = 360 - val * coefficient + h;
    } else {
        h = h - val * coefficient;
    }

    hsl_to_rgb(h,s,l, &r,&g,&b);

    pixel.x = r;
    pixel.y = g;
    pixel.z = b;
    write_imageui(output, pos, pixel);
}

__kernel void color_saturation(__read_only image2d_t input, __write_only image2d_t output,
                                const double val, int hue_start, int hue_end) {
    const int2 pos = (int2)(get_global_id(0), get_global_id(1));
    uint4 pixel = read_imageui(input, pos);

    __local unsigned int r;
    __local unsigned int g;
    __local unsigned int b;

    r = pixel.x;
    g = pixel.y;
    b = pixel.z;

    __local double h;
    __local double s;
    __local double l;

    h = 0;
    s = 0;
    l = 0;

    rgb_to_hsl(r,g,b, &h,&s,&l);

    int coefficient = 1;
    if (h > hue_end && h < hue_start) {
        coefficient = 1 - max((h - hue_end), (hue_start - h)) / 10;
        if (coefficient <= 0) {
            write_imageui(output, pos, pixel);
            return;
        }
    }

    double modifier = s * val*coefficient;

    if (s + modifier > 1) {
        s = 1;
    } else if (s + modifier < 0) {
        s = 0;
    } else {
        s += modifier;
    }

    hsl_to_rgb(h,s,l, &r,&g,&b);

    pixel.x = r;
    pixel.y = g;
    pixel.z = b;
    write_imageui(output, pos, pixel);
}

__kernel void color_lightness(__read_only image2d_t input, __write_only image2d_t output,
                                const double val, int hue_start, int hue_end) {
    const int2 pos = (int2)(get_global_id(0), get_global_id(1));
    uint4 pixel = read_imageui(input, pos);

    __local unsigned int r;
    __local unsigned int g;
    __local unsigned int b;

    r = pixel.x;
    g = pixel.y;
    b = pixel.z;

    __local double h;
    __local double s;
    __local double l;

    h = 0;
    s = 0;
    l = 0;

    rgb_to_hsl(r,g,b, &h,&s,&l);

    int coefficient = 1;
    if (h > hue_end && h < hue_start) {
        coefficient = 1 - max((h - hue_end), (hue_start - h)) / 10;
        if (coefficient <= 0) {
            write_imageui(output, pos, pixel);
            return;
        }
    }

    l *= val*coefficient;

    if (l > 1) {
        l = 1;
    } else if (l < 0) {
        l = 0;
    }

    hsl_to_rgb(h,s,l, &r,&g,&b);

    pixel.x = r;
    pixel.y = g;
    pixel.z = b;
    write_imageui(output, pos, pixel);
}
