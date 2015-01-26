#version 330

struct Light {
    vec3 position;
    vec3 color;
    float power;

    vec3 light_to_surface;
};

const int num_lights = 3;

in vec2 Texcoord;
in vec3 surface_normal;
in vec3 camera_to_surface;
in Light lights[num_lights];
in vec4 shadow_coord;
in vec3 Tangent;
in vec3 Bitangent;
in vec3 Normal;

out vec4 outColor;

uniform float time;
uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_map;
uniform sampler2D emissive_texture;
uniform sampler2D shadow_map;

vec4 diffuse;
vec4 specular;
vec4 normal;
vec4 emissive;

vec3 map_surface_normal;

const bool SHADOWS = true;
const bool NORMAL_DEBUG = false;

const vec2 poisson_disk[4] = vec2[]( vec2( -0.94201624, -0.39906216 ),
                                     vec2( 0.94558609, -0.76890725 ),
                                     vec2( -0.094184101, -0.92938870 ),
                                     vec2( 0.34495938, 0.29387760 ) );

vec4 lightFragment(vec3 light_vector, vec3 light_color, float light_power){
    float intensity = light_power / (pow(light_vector.x, 2) + pow(light_vector.y,
         2) + pow(light_vector.z, 2));

    float cosTheta = dot(normalize(map_surface_normal), normalize(light_vector));
    cosTheta = clamp(cosTheta, 0.0, 1.0);

    vec3 reflection = reflect(-normalize(light_vector), normalize(map_surface_normal));
    float cosAlpha = clamp(dot(normalize(camera_to_surface), reflection),
        0.0, 1.0);


    vec4 diffuse_component = vec4(diffuse.rgb * intensity * cosTheta, diffuse.w);

    float specularity = specular.w;
    vec4 specular_component = vec4(specular.rgb * specularity * intensity *
        pow(cosAlpha,15), specular.w);

    vec4 lit_component = vec4(light_color, 1.0) *
        (diffuse_component + specular_component);
    return lit_component;
}

float getShadowFactor(){
    // Shadows
    float bias = 0.005;

    float angle = dot(normalize(map_surface_normal),
        normalize(lights[0].light_to_surface));

    bool is_back_face = angle < 0.2;
    bool in_shadow_map = (shadow_coord.x >= 0.0) && (shadow_coord.x <= 1.0) &&
        (shadow_coord.y >= 0.0) && (shadow_coord.y <= 1.0);

    float visibility = 1.0;
    for (int i=0;i<4;i++){
        float light_depth = texture(shadow_map, shadow_coord.xy + poisson_disk[i]/700.0).z;
        float current_depth = shadow_coord.z - bias;

        if ((light_depth < current_depth) && in_shadow_map && !is_back_face){
            visibility -= 0.2;
        }
    }

    return visibility;
}

void main() {
    diffuse = texture(diffuse_texture, Texcoord);
    specular = texture(specular_texture, Texcoord);
    emissive = texture(emissive_texture, Texcoord);

    map_surface_normal = (texture(normal_map, Texcoord) * 2 - vec4(1, 1, 1, 0)).rgb;
    // map_surface_normal = surface_normal;

    float visibility;
    if (SHADOWS){
        visibility = getShadowFactor();
    } else{
        visibility = 1.0;
    }

    // Works fine
    Light light;
    vec4 lit_component = vec4(0.0, 0.0, 0.0, 0.0);

    // Direction light
    light = lights[0];
    lit_component = lit_component + visibility *
        lightFragment(light.light_to_surface, light.color, light.power);

    // Other lights
    light = lights[1];
    lit_component = lit_component +
        lightFragment(light.light_to_surface, light.color, light.power);
    light = lights[2];
    lit_component = lit_component +
        lightFragment(light.light_to_surface, light.color, light.power);

    vec4 ambient_component = vec4(0.1, 0.1, 0.1, 1.0) * diffuse;
    vec4 emissive_component = vec4(emissive.rgb, 1.0);

    vec4 texel = mix(lit_component + ambient_component, emissive_component,
        emissive.a);

    if (texel.a < 0.5){
        discard;
    }

    if (NORMAL_DEBUG){
        // outColor = vec4(map_surface_normal, 1.0);
        outColor = vec4((lights[0].light_to_surface + 1.0) / 2.0, 1.0);
    } else {
        outColor = texel;
    }
}
