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

uniform sampler2D heightmap;

void main() {
    Texcoord = texcoord;

    vec4 offset = texture(heightmap, texcoord);
    float avg_offset = 10.0 * ((offset.x + offset.y + offset.z) / 3.0);

    vec3 scaled_position = position * scale;
    vec4 model_position = model * vec4(scaled_position, 1.0);
    model_position.y += avg_offset;
    vec4 world_position = view * model_position;
    // Order is important on the multiplication!
    gl_Position = proj * world_position;

    // Directional Lighting
    // float light_height = 10.0;
    // float distance_to_light = light_height - world_position.y;
    // vec3 direction_vector = normalize(vec3(1.0, 1.0, 0.0));
    // vec4 light_position = vec4(direction_vector.xyz, 0.0);
    // vec4 light_temp = view * (light_position);
    // light_to_surface = distance_to_light * (light_temp).xyz;



    // Real directional lighting
    vec3 direction_vector = vec3(1.0, 1.0, 0.0);
    direction_vector = normalize(direction_vector);
    vec4 light_temp = view * vec4(direction_vector, 0.0);
    light_to_surface = light_temp.xyz;

    surface_normal = (view * model * vec4(normal, 0.0)).xyz;

    camera_to_surface = vec3(0,0,0) - (world_position).xyz;
}
