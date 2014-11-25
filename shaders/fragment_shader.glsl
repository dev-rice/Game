#version 330

in vec2 Texcoord;
in vec3 surface_normal;
in vec3 light_vector;
in vec3 viewing_vector;

out vec4 outColor;

uniform float time;
uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;

void main() {
    vec3 light_color = vec3(0.9, 0.9, 1.0);
    float cosTheta = dot(normalize(surface_normal), normalize(light_vector));
    cosTheta = clamp(cosTheta, 0.0, 1.0);
    float intensity = 10 / (pow(light_vector.x, 2) + pow(light_vector.y, 2) + pow(light_vector.z, 2));

    vec3 reflection = reflect(-normalize(light_vector), normalize(surface_normal));
    float cosAlpha = clamp(dot(normalize(viewing_vector), reflection), 0.0, 1.0);


    vec4 ambient_component = vec4(0.0, 0.0, 0.0, 1.0);

    vec4 diffuse_component = intensity * cosTheta * vec4(light_color, 1.0) * texture(diffuse_texture, Texcoord);

    float specularity = texture(specular_texture, Texcoord).w;
    vec4 specular_component = specularity * pow(cosAlpha, 10) * vec4(light_color, 1.0) * texture(specular_texture, Texcoord);

    outColor = diffuse_component + specular_component + ambient_component;
}