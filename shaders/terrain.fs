#version 330

in vec2 Texcoord;
in vec3 light_to_surface;
in vec3 surface_normal;
in vec3 camera_to_surface;

out vec4 outColor;

uniform float time;
uniform sampler2D diffuse_texture;
uniform sampler2D heightmap;


vec4 lightFragment(vec3 light_vector, vec3 light_color, float light_power){
    float intensity = light_power / (pow(light_vector.x, 2) + pow(light_vector.y, 2) + pow(light_vector.z, 2));

    float cosTheta = dot(normalize(surface_normal), normalize(light_vector));
    cosTheta = clamp(cosTheta, 0.0, 1.0);

    vec3 reflection = reflect(-normalize(light_vector), normalize(surface_normal));
    float cosAlpha = clamp(dot(normalize(camera_to_surface), reflection), 0.0, 1.0);

    vec4 diffuse = texture(diffuse_texture, Texcoord);
    // vec4 specular = texture(specular_texture, Texcoord);
    //
    vec4 ambient_component = vec4(0.1, 0.1, 0.1, 1.0) * diffuse;

    vec4 diffuse_component = vec4(diffuse.rgb * intensity * cosTheta, diffuse.w);

    // float specularity = specular.w;
    // vec4 specular_component = vec4(specular.rgb * specularity * intensity * pow(cosAlpha,15), specular.w);

    // vec4 lit_component = vec4(light_color, 1.0) * (diffuse_component + specular_component + ambient_component);
    vec4 lit_component = vec4(light_color, 1.0) * (diffuse_component + ambient_component);
    return lit_component;
}

void main() {
    vec3 light_color = vec3(1.0, 1.0, 1.0);
    float light_power = 10.0;
    vec4 lit_component = lightFragment(light_to_surface, light_color, light_power);

    // vec4 emissive = texture(emissive_texture, Texcoord);
    // vec4 emissive_component = vec4(emissive.rgb, 1.0);

    // vec4 texel = mix(lit_component, emissive_component, emissive.a);
    // vec4 texel = lit_component;
    // vec4 texel = texture(heightmap, Texcoord);
    vec4 texel = texture(heightmap, Texcoord);

    if (texel.a < 0.5){
        discard;
    }
    outColor = texel;
}
