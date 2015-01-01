#version 330 core

// Input vertex data, different for all executions of this shader.
in vec3 position;

// Values that stay constant for the whole mesh.
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main(){
    gl_Position =  proj * view * model * vec4(position, 1.0);
}
