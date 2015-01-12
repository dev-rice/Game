#version 330

layout(location=1) in vec3 position;
layout(location=2) in vec3 normal;
layout(location=3) in vec2 texcoord;

layout(std140) uniform GlobalMatrices {
    mat4 view;
    mat4 proj;
};

uniform mat4 model;

void main(){
    // Supress some warnings for unused variables
    vec3 n = normal;
    vec2 t = texcoord;

    gl_Position = proj * inverse(view) * vec4(position, 1.0);
}
