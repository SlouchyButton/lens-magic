__constant sampler_t smplr = CLK_FILTER_NEAREST | CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE;

__kernel void entry(__read_only image2d_t input, __write_only image2d_t output, const double val) {
    const int2 pos = (int2)(get_global_id(0), get_global_id(1));
    float4 pixel = read_imagef(input, smplr, pos);

    pixel = pixel * (float)pow(2, val);
    write_imagef(output, pos, pixel);
}