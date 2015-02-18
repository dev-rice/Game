#version 330

in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D base_texture;
uniform vec3 textColor;

void main() {
    vec4 base = texture(base_texture, Texcoord);
    outColor = vec4(textColor * base.rgb, base.a);
}
