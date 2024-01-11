#version 330 core
in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D image;
uniform float value;

vec4 rgb_to_hsl(vec4 rgb) {
   vec4 hsl = rgb;

   float max_val = max(rgb.r, max(rgb.g, rgb.b));
   float min_val = min(rgb.r, min(rgb.g, rgb.b));
   float delta = max_val - min_val;

   if (delta == 0) {
      return vec4(0,0,rgb.r,rgb.a);
   }
   
   if (max_val == rgb.r) {
      hsl.x = 60 * mod(((rgb.g-rgb.b)/delta), 6);
   }
   if (max_val == rgb.g) {
      hsl.x = 60 * ((rgb.b-rgb.r)/delta + 2);
   }
   if (max_val == rgb.b) {
      hsl.x = 60 * ((rgb.r-rgb.g)/delta + 4);
   }

   if (hsl.x < 0) {
      hsl.x = 360+hsl.x;
   }

   hsl.z = (max_val + min_val) / 2;
   hsl.y = delta / (1 - abs(2*(hsl.z) - 1));

   return hsl;
}

vec4 hsl_to_rgb(vec4 hsl) {
   float c = hsl.y * (1 - abs(2*hsl.z-1));
   float x = c * (1 - abs(mod(hsl.x/60, 2) - 1));
   float m = hsl.z - c/2;

   float r = 0;
   float g = 0;
   float b = 0;

   if (hsl.x >= 0 && hsl.x < 60) {
      r = c;
      g = x;
   }
   if (hsl.x >= 60 && hsl.x < 120) {
      r = x;
      g = c;
   }
   if (hsl.x >= 120 && hsl.x < 180) {
      g = c;
      b = x;
   }
   if (hsl.x >= 180 && hsl.x < 240) {
      g = x;
      b = c;
   }
   if (hsl.x >= 240 && hsl.x < 300) {
      r = x;
      b = c;
   }
   if (hsl.x >= 300 && hsl.x < 360) {
      r = c;
      b = x;
   }

   return vec4(r+m, g+m, b+m, hsl.w);
}

void main() {
   vec4 texColor = texture(image, TexCoord);
   
   vec4 texHSL = rgb_to_hsl(texColor);

   float coefficient = (1 - texHSL.z * 2.5) * -1;
   if (coefficient < 0) {
      coefficient = 0;
   }

   float modifier = (((value * -1) * coefficient) + 1);

   FragColor = modifier * (texColor-1) + 1;
}