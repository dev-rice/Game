#version 330

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec2 Texcoord;
out vec3 surface_normal;
out vec3 light_vector;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float time;
uniform float scale;

uniform vec3 light_position;

void main() {
    surface_normal = normal;
    Texcoord = texcoord;

    // Order is important on the multiplication!
    vec4 world_position = view * model * vec4(position * scale, 1.0);
    vec4 final_position = proj * world_position;

    vec4 temp_light = vec4(light_position, 1.0);
    light_vector = vec3(temp_light.x, temp_light.y, temp_light.z);

    // Wrapping effect
    // gl_Position = vec4(final_position.x, final_position.y + pow(1.25, final_position.z + 2), final_position.z, final_position.w);
    
    // Wavy ground effect
    // gl_Position = vec4(final_position.x, final_position.y + (0.5*sin(time + final_position.z)), final_position.z, final_position.w);

    // Boring normal effect
    gl_Position = final_position;

}