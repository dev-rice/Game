#version 330

in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D base_texture;
uniform vec3 textColor;

void main() {
    outColor = texture(base_texture, Texcoord);
}