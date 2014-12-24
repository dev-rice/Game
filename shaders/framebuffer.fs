#version 150

in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D base_texture;

void main() {
    // outColor = vec4(1 - texture(base_texture, Texcoord).rgb, texture(base_texture, Texcoord).a);
    outColor = texture(base_texture, Texcoord);
}
