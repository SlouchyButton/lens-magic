#version 330 core
in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D image;
uniform float value;
void main() {
    vec4 texColor = texture(image, TexCoord);
    FragColor = (value+1) * (texColor-1) + 1;
}