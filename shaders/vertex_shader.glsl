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

void main() {
    Color = color;
    Texcoord = texcoord;
    // Order is important on the multiplication!
    world_position = proj * view * model * vec4(position, 1.0);
    gl_Position = world_position;
}