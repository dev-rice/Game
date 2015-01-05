#version 330

const int num_lights = 3;

struct Light {
    vec3 position;
    vec3 color;
    float power;

    vec3 light_to_surface;
};

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec2 Texcoord;
out vec3 surface_normal;
out vec3 camera_to_surface;
out Light lights[num_lights];
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
    lights[0].position = vec3(-1.0, 1.0, 0.0);
    lights[0].color = vec3(1.0, 1.0, 1.0);
    lights[0].power = 1.0;
    vec3 direction_vector = normalize(lights[0].position);
    vec4 light_vector = view * vec4(direction_vector, 0.0);
    lights[0].light_to_surface = light_vector.xyz;

    lights[1].position = vec3(-1.5, 0.5, 0.0);
    lights[1].color = vec3(1.0, 0.3, 0.1);
    lights[1].power = 5.0;

    lights[2].position = vec3(-4.5, 5.0, sin(5*time));
    lights[2].color = vec3(1.0, 1.0, 1.0);
    lights[2].power = 0.0;

    // The first light is reserved for the directional light
    for (int i = 1; i < num_lights; ++i){
        vec3 light_vector = ((view * vec4(lights[i].position, 1.0)) - (world_position)).xyz;
        lights[i].light_to_surface = light_vector;
    }

    surface_normal = (view * model * vec4(normal, 0.0)).xyz;
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
