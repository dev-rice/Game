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
uniform float plane_rotation;

void main(){
    Texcoord = texcoord;

    vec3 scaled_position =  scale * position;

    mat4 rotation_matrix = mat4( view[0][0], view[1][0], view[2][0], 0.0,
                                 view[0][1], view[1][1], view[2][1], 0.0,
                                 view[0][2], view[1][2], view[2][2], 0.0,
                                 0.0       , 0.0       , 0.0       , 1.0);

    vec4 world_position = rotation_matrix * vec4(scaled_position, 1.0);
    world_position = model * world_position;

    gl_Position = proj * view * world_position;
    
}
