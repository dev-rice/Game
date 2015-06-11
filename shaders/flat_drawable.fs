#version 150

in vec2 Texcoord;

out vec4 outColor;

uniform float opacity;
uniform sampler2D base_texture;

void main() {
    vec4 texel = texture(base_texture, Texcoord);
    outColor = vec4(texel.rgb, opacity * texel.a);
}
