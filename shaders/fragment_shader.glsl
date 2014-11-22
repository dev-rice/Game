#version 330

in vec2 Texcoord;
in vec3 surface_normal;
in vec3 light_vector;

out vec4 outColor;

uniform float time;
uniform sampler2D tex;
uniform bool has_texture;

void main() {
    vec3 light_color = vec3(0.4, 0.6, 0.8);
    float cosTheta = dot(normalize(surface_normal), normalize(light_vector));
    float intensity = 10 / (pow(light_vector.x, 2) + pow(light_vector.y, 2) + pow(light_vector.z, 2));

    outColor = texture(tex, Texcoord) * vec4(cosTheta * light_color * intensity, 1.0);
}