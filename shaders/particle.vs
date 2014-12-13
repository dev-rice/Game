#version 330

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec2 Texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float time;
uniform float scale;

void main(){
    Texcoord = texcoord;
    vec4 world_position = model * vec4(position, 1.0);

    gl_Position = proj * view * world_position;
}