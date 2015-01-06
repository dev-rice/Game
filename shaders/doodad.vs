#version 330

layout(location=1) in vec3 position;
layout(location=2) in vec3 normal;
layout(location=3) in vec2 texcoord;

out vec2 Texcoord;
out vec3 surface_normal;
out vec3 camera_to_surface;
out vec3 light_to_surface;
out vec4 shadow_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float time;
uniform float scale;

uniform mat4 depth_view;
uniform mat4 depth_proj;

void main() {
    Texcoord = texcoord;

    vec3 scaled_position = position * scale;
    vec4 model_position = model * vec4(scaled_position, 1.0);
    vec4 world_position = view * model_position;
    // Order is important on the multiplication!
    gl_Position = proj * world_position;

    // Real directional lighting
    vec3 light_position = vec3(-1.0, 1.0, 0.0);
    vec3 direction_vector = normalize(light_position);
    vec4 light_vector = view * vec4(direction_vector, 0.0);
    light_to_surface = light_vector.xyz;

    surface_normal = (view * model * vec4(normal, 0.0)).xyz;
    // surface_normal = normal;
    camera_to_surface = vec3(0,0,0) - (world_position).xyz;

    // Shadow shtuff
    mat4 bias_matrix = mat4( 0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0 );

    mat4 depth_matrix = depth_proj * depth_view * model;
    depth_matrix = bias_matrix * depth_matrix;
    shadow_coord = depth_matrix * vec4(scaled_position, 1.0);
}
