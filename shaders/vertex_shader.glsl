#version 330

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec2 Texcoord;
out vec3 surface_normal;
out vec3 light_vector;
out vec3 viewing_vector;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float time;
uniform float scale;

uniform vec3 light_position;

void main() {
    Texcoord = texcoord;

    vec3 scaled_position = position * scale;

    // Order is important on the multiplication!
    gl_Position = proj * view * model * vec4(scaled_position, 1.0);

    vec4 light_vector_temp = (view * vec4(light_position, 1.0)) - (view * model * vec4(scaled_position, 1.0));
    light_vector = vec3(light_vector_temp.xyz);

    vec4 surface_normal_temp = view * model * vec4(normal, 0.0);
    surface_normal = vec3(surface_normal_temp.xyz);

    viewing_vector = vec3(0,0,0) - (view * model * vec4(scaled_position, 1.0)).xyz;
}