#version 330

in vec3 position;
in vec3 color;
in vec2 texcoord;

out vec3 Color;
out vec2 Texcoord;
out vec4 world_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float time;
uniform float scale;

void main() {
    Color = color;
    Texcoord = texcoord;

    // Order is important on the multiplication!
    world_position = proj * view * model * vec4(position * scale, 1.0);

    // Wrapping effect
    gl_Position = vec4(world_position.x, world_position.y + pow(1.25, world_position.z + 2) + (0.5*sin(time + world_position.z)), world_position.z, world_position.w);
    
    // Wavy ground effect
    // gl_Position = vec4(world_position.x, world_position.y + (0.5*sin(time + world_position.z)), world_position.z, world_position.w);

    // Boring normal effect
    // gl_Position = world_position;

}