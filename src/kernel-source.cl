__kernel void rgb_to_hsl(unsigned char r, unsigned char g, unsigned char b,
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
				__local unsigned char* r, __local unsigned char* g, __local unsigned char* b) {
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

__kernel void exposure(__global unsigned char* input, __global unsigned char* output, const double val) {
	int i = get_global_id(0);
	for (int j = 0; j < 3; j++) {
		int new = input[i+j] * pow(2, val);
		if (new > 255) {
			output[i] = 255;
			output[i+1] = 255;
			output[i+2] = 255;
			break;
		} else if (new < 0) {
			output[i] = 0;
			output[i+1] = 0;
			output[i+2] = 0;
			break;
		} else {
			output[i+j] = new;
		}
	}
}

__kernel void brightness(__global unsigned char* input, __global unsigned char* output, const double val) {
	int i = get_global_id(0);
	for (int j = 0; j < 3; j++) {
        int new = input[i+j] + val;
        if (new > 255) {
          output[i] = 255;
          output[i+1] = 255;
          output[i+2] = 255;
          break;
        } else if (new < 0) {
          output[i] = 0;
          output[i+1] = 0;
          output[i+2] = 0;
          break;
        } else {
          output[i+j] = new;
        }
	}
}

__kernel void contrast(__global unsigned char* input, __global unsigned char* output, const double val) {
	int i = get_global_id(0);
	for (int j = 0; j < 3; j++) {
		int new = val*(input[i+j]-128)+128;
		if (new > 255) {
			output[i] = 255;
			output[i+1] = 255;
			output[i+2] = 255;
			break;
		} else if (new < 0) {
			output[i] = 0;
			output[i+1] = 0;
			output[i+2] = 0;
			break;
		} else {
			output[i+j] = new;
		}
	}
}

__kernel void saturation(__global unsigned char* input, __global unsigned char* output, const double val) {
	int i = get_global_id(0);

	__local unsigned char r;
	__local unsigned char g;
	__local unsigned char b;

	r = input[i];
	g = input[i+1];
	b = input[i+2];

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

    output[i] = r;
    output[i+1] = g;
    output[i+2] = b;
}
