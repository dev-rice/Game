#version 330

in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D base_texture;
uniform vec3 textColor;

const bool TEXT_DEBUG = false;

void main() {
    vec4 base = texture(base_texture, Texcoord);

    if (TEXT_DEBUG){
        // Debug
        outColor = vec4(0.0, 0.0, 0.0, 1.0) + vec4(base.r, base.r, base.r, 0.0) * vec4(textColor, 1.0);
    } else {
        // Regular
        outColor = vec4(1.0, 1.0, 1.0, base.r) * vec4(textColor, 1.0);
    }

}
