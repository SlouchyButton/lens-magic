#version 330 core
in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D image;
uniform float value;
void main() {
   vec4 texColor = texture(image, TexCoord);
   texColor.r += value;
   texColor.b -= value;
   FragColor = texColor;
}