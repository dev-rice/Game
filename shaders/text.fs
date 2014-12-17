#version 330

in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D texFramebuffer;

void main() {
    outColor = vec4(texture(texFramebuffer, Texcoord).rgb, texture(texFramebuffer, Texcoord).a);
}