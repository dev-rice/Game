#version 330

in vec3 Color;
in vec2 Texcoord;
in vec4 world_position;

out vec4 outColor;

uniform sampler2D tex;
uniform bool has_texture;

void main() {
    // float intensity = 1.0;
    float intensity = 200 * (1 / (pow(world_position.x,2) + pow(world_position.y,2) + pow(world_position.z,2)));
    intensity = min(intensity, 1.0);
    
    if (has_texture){
        outColor = texture(tex, Texcoord) * vec4(Color * intensity, 1.0);
    } else {
        outColor = vec4(0.0, 1.0, 0.0, 1.0);
    }
}