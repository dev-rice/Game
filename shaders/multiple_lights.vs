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
    lights[0].position = vec3(-1.5, 0.5, 0.0);
    lights[0].color = vec3(1.0, 0.0, 0.0);
    lights[0].power = 5.0;

    lights[1].position = vec3(-4.5, 5.0, sin(5*time));
    lights[1].color = vec3(1.0, 1.0, 1.0);
    lights[1].power = 25.0;

    lights[2].position = vec3(0.0, 100.0, 0.0);
    lights[2].color = vec3(1.0, 1.0, 1.0);
    lights[2].power = 1000.0;


    for (int i = 0; i < num_lights; ++i){
        vec3 light_vector = ((view * vec4(lights[i].position, 1.0)) - (view * model * vec4(scaled_position, 1.0))).xyz;
        lights[i].light_to_surface = light_vector;
    }

    surface_normal = (view * model * vec4(normal, 0.0)).xyz;

    camera_to_surface = vec3(0,0,0) - (view * model * vec4(scaled_position, 1.0)).xyz;
}