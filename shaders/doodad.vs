#version 330

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec2 Texcoord;
out vec3 light_to_surface;
out vec3 surface_normal;
out vec3 camera_to_surface;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float time;
uniform float scale;

void main() {
    Texcoord = texcoord;

    vec3 scaled_position = position * scale;
    vec4 world_position = model * vec4(scaled_position, 1.0);
    // Order is important on the multiplication!
    gl_Position = proj * view * world_position;

    // Point lighting
    // vec3 light_position = vec3(-1.5, 0.5 , 0.0);
    // vec4 light_temp = (view * vec4(light_position, 1.0)) - (view * model * vec4(scaled_position, 1.0));
    // light_to_surface = light_temp.xyz;

    // Directional Lighting
    float light_height = 10.0;
    float distance_to_light = light_height - world_position.y;
    vec3 direction_vector = normalize(vec3(1.0, 1.0, 0.0));
    vec4 light_position = vec4(direction_vector.xyz, 0.0);
    vec4 light_temp = view * (light_position);
    light_to_surface = distance_to_light * (light_temp).xyz;

    surface_normal = (view * model * vec4(normal, 0.0)).xyz;

    camera_to_surface = vec3(0,0,0) - (view * model * vec4(scaled_position, 1.0)).xyz;
}