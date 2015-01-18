#version 330

const int num_lights = 3;

struct Light {
    vec3 position;
    vec3 color;
    float power;

    vec3 light_to_surface;
};

layout(location=1) in vec3 position;
layout(location=2) in vec3 normal;
layout(location=3) in vec3 tangent;
layout(location=4) in vec3 bitangent;
layout(location=5) in vec2 texcoord;

out vec2 Texcoord;
out vec3 surface_normal;
out vec3 camera_to_surface;
out Light lights[num_lights];
out vec4 shadow_coord;
out vec3 Tangent;
out vec3 Bitangent;
out vec3 Normal;


layout(std140) uniform GlobalMatrices {
    mat4 view;
    mat4 proj;
};

layout(std140) uniform ShadowMatrices {
    mat4 depth_view;
    mat4 depth_proj;
};

layout(std140) uniform Mouse {
    vec3 mouse_point;
};

uniform mat4 model;

uniform float time;
uniform float scale;

void main() {
    Texcoord = texcoord;

    Tangent = tangent;
    Bitangent = bitangent;
    Normal = normal;

    vec4 normal_world = view * (model * vec4(normal, 0.0));
    normal_world.w = 0.0;
    vec4 tangent_world = view * (model * vec4(tangent, 0.0));
    tangent_world.w = 0.0;
    vec4 bitangent_world = view * (model * vec4(bitangent, 0.0));
    bitangent_world.w = 0.0;

    mat4 normal_basis = mat4(tangent_world,
                             bitangent_world,
                             normal_world,
                             0, 0, 0  , 1);
    normal_basis = inverse(normal_basis);

    vec3 scaled_position = position * scale;
    vec4 model_position = model * vec4(scaled_position, 1.0);
    vec4 world_position = view * model_position;
    // Order is important on the multiplication!
    gl_Position = proj * world_position;

    // Real directional lighting
    lights[0].position = vec3(-1.0, 1.0, 0.5);
    lights[0].color = vec3(1.0, 1.0, 1.0);
    lights[0].power = 1.0;
    vec3 direction_vector = normalize(lights[0].position);
    vec4 light_vector = view * vec4(direction_vector, 0.0);
    light_vector = normalize(normal_basis * light_vector);
    lights[0].light_to_surface = light_vector.xyz;

    lights[1].position = vec3(-1.415, 0.5, 0.0);
    lights[1].color = vec3(1.0, 0.3, 0.1);
    lights[1].power = 5.0;

    lights[2].position = vec3(mouse_point.x, mouse_point.y + 1.0, mouse_point.z);
    lights[2].color = vec3(0.0, 1.0, 0.0);
    lights[2].power = 10.0;

    // The first light is reserved for the directional light
    for (int i = 1; i < num_lights; ++i){
        vec3 light_vector = (normal_basis * (view * (vec4(lights[i].position, 1.0)) -
            (world_position))).xyz;

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