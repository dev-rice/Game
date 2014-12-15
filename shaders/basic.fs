#version 150

in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D texFramebuffer;
uniform bool is_outline;
void main() {
    if (is_outline){
        outColor = vec4(1.0, 0.0, 1.0, 1.0);

    } else {
        vec3 color = texture(texFramebuffer, Texcoord).rgb;
        outColor = vec4(1 - color, 1.0);
    }
}