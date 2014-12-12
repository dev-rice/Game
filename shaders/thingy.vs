#version 330

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec2 Texcoord;
out vec3 surface_normal;
out vec3 light_vector;
out vec3 viewing_vector;
out Light main_light;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float time;
uniform float scale;

uniform Light light;

void main() {
    Texcoord = texcoord;
    main_light = light;

    vec3 scaled_position = position * scale;
    vec4 world_position = model * vec4(scaled_position, 1.0);
    // Order is important on the multiplication!
    gl_Position = proj * view * world_position;

    // 
    // vec4 light_to_surface = (view * vec4(light.position, 1.0)) - (view * model * vec4(scaled_position, 1.0));
    // light_vector = light_to_surface.xyz;

    // Directional Lighting
    float light_height = 10.0;
    float distance_to_light = light_height - world_position.y;
    vec3 direction_vector = normalize(vec3(1.0, 1.0, 0.0));
    vec4 light_position = vec4(direction_vector.xyz, 0.0);
    vec4 light_to_surface = view * (light_position);
    light_vector = distance_to_light * (light_to_surface).xyz;


    vec4 surface_normal_temp = view * model * vec4(normal, 0.0);
    surface_normal = vec3(surface_normal_temp.xyz);

    viewing_vector = vec3(0,0,0) - (view * model * vec4(scaled_position, 1.0)).xyz;
}