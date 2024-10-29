__constant sampler_t smplr = CLK_FILTER_NEAREST | CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE;

__kernel void entry(__read_only image2d_t input, __write_only image2d_t output,
        const int width, const int height, const int preview_width,
        const int preview_height) {
    const int2 pos = (int2)(get_global_id(0), get_global_id(1));
    const float2 factor = (float2)(preview_width/(float)width, preview_height/(float)height);

    float4 pixel = read_imagef(input, smplr, pos);

    // Read the 4 pixels from the input image
    float4 pixel2 = read_imagef(input, smplr, pos + (int2)(1, 0));
    float4 pixel3 = read_imagef(input, smplr, pos + (int2)(0, 1));
    float4 pixel4 = read_imagef(input, smplr, pos + (int2)(1, 1));

    //printf("  | (%d,%d,%d,%d) %dX%d [%.1fX%.1f] %dX%d |  ", pixel.x, pixel.y, pixel.z, pixel.w, pos.x, pos.y, factor.x, factor.y, (int)(pos.x * factor.x), (int)(pos.y * factor.y));

    write_imagef(output, (int2)(pos.x * factor.x, pos.y * factor.y), (pixel + pixel2 + pixel3 + pixel4) / 4);
    /*for(int x = 0; x < factor.x; x++)
    {
        for(int y = 0; y < factor.y; y++)
        {
            write_imagef(output, (int2)(pos.x * factor.x + x, pos.y * factor.y +
            y), pixel);
        }
    }*/
}