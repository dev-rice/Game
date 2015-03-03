#version 150

in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D base_texture;
uniform float opacity;

void main() {
    vec4 texel = texture(base_texture, Texcoord);
    outColor = vec4(texel.rgb, opacity * texel.a);
}
