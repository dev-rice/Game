#version 330

layout(location=1) in vec3 position;
layout(location=2) in vec3 normal;
layout(location=3) in vec2 texcoord;

out vec3 world_position;
out vec3 click_point;

layout(std140) uniform GlobalMatrices {
    mat4 view;
    mat4 proj;
};

layout(std140) uniform Mouse {
    vec3 mouse_point;
};


uniform mat4 model;
uniform float scale;

void main(){
    click_point = mouse_point;
    // Supress some warnings for unused variables
    vec3 n = normal;
    vec2 t = texcoord;

    world_position = (model * vec4(scale * position, 1.0)).xyz;

    gl_Position = proj * view * model * vec4(scale * position, 1.0);
}
