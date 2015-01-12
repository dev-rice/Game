#version 330

layout(location=1) in vec3 position;
layout(location=2) in vec3 normal;
layout(location=3) in vec2 texcoord;

out float distance_to_click;
out vec3 world_position;

layout(std140) uniform GlobalMatrices {
    mat4 view;
    mat4 proj;
};

uniform mat4 model;
uniform float scale;

void main(){
    // Supress some warnings for unused variables
    vec3 n = normal;
    vec2 t = texcoord;

    world_position = (view * model * vec4(scale * position, 1.0)).xyz;

    gl_Position = proj * view * model * vec4(scale * position, 1.0);
}
