#version 330

in vec3 Color;
in vec2 Texcoord;
in vec4 world_position;

out vec4 outColor;

uniform sampler2D tex;

void main() {
    // float intensity = 1.0;
    float intensity = 200 * (1 / (pow(world_position.x,2) + pow(world_position.y,2) + pow(world_position.z,2)));
    intensity = min(intensity, 1.0);
    
    outColor = texture(tex, Texcoord) * vec4(Color * intensity, 1.0);
    // outColor = vec4(Color, 1.0);
}