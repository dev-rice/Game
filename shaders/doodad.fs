#version 330

in vec2 Texcoord;
in vec3 surface_normal;
in vec3 camera_to_surface;
in vec3 light_to_surface;
in vec4 shadow_coord;

out vec4 outColor;

uniform float time;
uniform float scale;

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_map;
uniform sampler2D emissive_texture;
uniform sampler2D shadow_map;

const bool SHADOW = true;

void main() {
    vec3 light_color = vec3(1.0, 1.0, 1.0);
    float light_power = 1.0;

    vec4 diffuse = texture(diffuse_texture, Texcoord);
    vec4 emissive = texture(emissive_texture, Texcoord);

    vec4 ambient_component = vec4(0.1, 0.1, 0.1, 1.0) * diffuse;
    vec4 emissive_component = vec4(emissive.rgb, 1.0);

    // float intensity = light_power / (pow(light_to_surface.x, 2) +
        // pow(light_to_surface.y, 2) + pow(light_to_surface.z, 2));

    float intensity = 1.0;

    vec3 normal_surface_normal = normalize(surface_normal);
    vec3 normal_light = normalize(light_to_surface);
    float cosTheta = dot(normal_surface_normal, normal_light);

    // cosTheta = clamp(cosTheta, 0.0, 1.0);
    // cosTheta = 1.0;
    vec4 diffuse_component = vec4(diffuse.rgb * cosTheta, diffuse.a);

    // vec4 lit_component = vec4(light_color, 1.0) * diffuse_component;

    float bias = 0.005;
    float visibility = 1.0;
    if ( texture( shadow_map, shadow_coord.xy).z  <=  shadow_coord.z - bias){
        visibility = 0.5;
    }

    // vec4 texel = mix(lit_component + ambient_component, emissive_component,
        // emissive.a);
    vec4 texel = diffuse_component;
    if (texel.a < 0.5){
        discard;
    }

    if (SHADOW){
        outColor = vec4(visibility * texel.rgb, texel.a);
        // outColor = vec4(shadow_coord.z * texel.rgb, texel.a);
        // outColor = vec4(0.0, 0.0, texture(shadow_map, shadow_coord.xy).r, 1.0);
        // outColor = texture(shadow_map, shadow_coord.xy);

    } else {
        // texel = emissive;
        // outColor = texel;
        outColor = vec4(surface_normal, 1.0);
    }

}
