#version 330 core
in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D image;
void main() {
    FragColor = texture(image, TexCoord);
}