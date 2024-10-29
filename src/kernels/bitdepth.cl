__constant sampler_t smplr = CLK_FILTER_NEAREST | CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE;

__kernel void entry(__read_only image2d_t input, __write_only image2d_t output) {
    const int2 pos = (int2)(get_global_id(0), get_global_id(1));
    float4 pixel = read_imagef(input, smplr, pos);
    uint4 upixel = (uint4)(pixel.x * 65535, pixel.y * 65535, pixel.z * 65535, pixel.w * 65535);
    uint4 pixout = (upixel * 255 + 32895) >> 16;
    pixout.w = 255;
    write_imageui(output, pos, pixout);
}