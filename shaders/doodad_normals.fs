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
    float visibility;

    float angle = dot(normalize(map_surface_normal),
        normalize(lights[0].light_to_surface));

    vec4 n_pixel  = textureOffset(shadow_map, shadow_coord.xy, ivec2(0, 1));
    vec4 s_pixel  = textureOffset(shadow_map, shadow_coord.xy, ivec2(0, -1));
    vec4 e_pixel  = textureOffset(shadow_map, shadow_coord.xy, ivec2(1, 0));
    vec4 w_pixel  = textureOffset(shadow_map, shadow_coord.xy, ivec2(-1, 0));
    vec4 nw_pixel = textureOffset(shadow_map, shadow_coord.xy, ivec2(-1,-1));
    vec4 ne_pixel = textureOffset(shadow_map, shadow_coord.xy, ivec2( 1,-1));
    vec4 sw_pixel = textureOffset(shadow_map, shadow_coord.xy, ivec2(-1, 1));
    vec4 se_pixel = textureOffset(shadow_map, shadow_coord.xy, ivec2( 1, 1));
    vec4 middle_pixel = texture(shadow_map, shadow_coord.xy);

    vec4 blurred_pixel = (n_pixel + s_pixel + e_pixel + w_pixel + nw_pixel +
        ne_pixel + sw_pixel + se_pixel + middle_pixel) / 9.0;

    vec4 shadow_texture = blurred_pixel;

    bool in_shadow_map = (shadow_coord.x >= 0.0) && (shadow_coord.x <= 1.0) &&
    (shadow_coord.y >= 0.0) && (shadow_coord.y <= 1.0);
    float light_depth = shadow_texture.z;
    float current_depth = shadow_coord.z - bias;
    if ((light_depth  <=  current_depth) && in_shadow_map && (angle > 0.2)){
        visibility = 0.2;
    } else {
        visibility = 1.0;
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
    // outColor = texel;
    // map_surface_normal = vec3(abs(map_surface_normal.x), abs(map_surface_normal.y),
    //     abs(map_surface_normal.z));
    outColor = vec4((lights[0].light_to_surface + 1.0) / 2.0, 1.0);
}
