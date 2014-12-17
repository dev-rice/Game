#version 330

in vec2 position;
in vec2 texcoord;

out vec2 Texcoord;

void main() {
    Texcoord = vec2(texcoord.x, 1.0 - texcoord.y);
    vec3 position_temp = vec3(position, 1.0);
    gl_Position = vec4(position_temp.xy, 0.0, 1.0);
}