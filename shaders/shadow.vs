#version 330 core

// Input vertex data, different for all executions of this shader.
layout (location=1) in vec3 position;
layout (location=2) in vec3 normal;
layout (location=3) in vec2 texcoord;

// Values that stay constant for the whole mesh.
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform float scale;

void main(){
    gl_Position =  proj * view * model * vec4(scale * position, 1.0);
}
