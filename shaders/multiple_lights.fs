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

const bool DEBUG_SHADOW = true;

vec4 lightFragment(vec3 light_vector, vec3 light_color, float light_power){
    float intensity = light_power / (pow(light_vector.x, 2) + pow(light_vector.y, 2) + pow(light_vector.z, 2));

    float cosTheta = dot(normalize(surface_normal), normalize(light_vector));
    cosTheta = clamp(cosTheta, 0.0, 1.0);

    vec3 reflection = reflect(-normalize(light_vector), normalize(surface_normal));
    float cosAlpha = clamp(dot(normalize(camera_to_surface), reflection), 0.0, 1.0);


    vec4 diffuse_component = vec4(diffuse.rgb * intensity * cosTheta, diffuse.w);

    float specularity = specular.w;
    vec4 specular_component = vec4(specular.rgb * specularity * intensity * pow(cosAlpha,15), specular.w);

    vec4 lit_component = vec4(light_color, 1.0) * (diffuse_component + specular_component);
    return lit_component;
}

void main() {
    diffuse = texture(diffuse_texture, Texcoord);
    specular = texture(specular_texture, Texcoord);
    emissive = texture(emissive_texture, Texcoord);

    // For some reason using a for loop like this makes the framerate choppy.
    // The enumeration below behaves fine.
    // int i = 0;
    // vec4 lit_component = vec4(0.0, 0.0, 0.0, 0.0);
    // Light light;
    // for (i = 0; i < num_lights; ++i){
    //     Light light = lights[i];
    //     lit_component = lit_component + lightFragment(light.light_to_surface, light.color, light.power);
    // }

    // Works fine
    Light light;
    vec4 lit_component = vec4(0.0, 0.0, 0.0, 0.0);

    light = lights[0];
    lit_component = lit_component + lightFragment(light.light_to_surface, light.color, light.power);
    light = lights[1];
    lit_component = lit_component + lightFragment(light.light_to_surface, light.color, light.power);
    light = lights[2];
    lit_component = lit_component + lightFragment(light.light_to_surface, light.color, light.power);

    vec4 ambient_component = vec4(0.1, 0.1, 0.1, 1.0) * diffuse;
    vec4 emissive_component = vec4(emissive.rgb, 1.0);

    float bias = 0.005;
    float visibility = 1.0;
    if ( texture( shadow_map, shadow_coord.xy).z  <  shadow_coord.z - bias){
        visibility = 0.5;
    }

    vec4 texel = mix(lit_component + ambient_component, emissive_component, emissive.a);
    if (texel.a < 0.5){
        discard;
    }
    outColor = texel;

    if (DEBUG_SHADOW){
        outColor = vec4(visibility * texel.rgb, texel.a);
        // outColor = vec4(shadow_coord.z * texel.rgb, texel.a);
        // outColor = vec4(0.0, 0.0, texture(shadow_map, shadow_coord.xy).r, 1.0);
        // outColor = texture(shadow_map, shadow_coord.xy);

    }

}
