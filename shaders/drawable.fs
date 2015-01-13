#version 330

in vec3 world_position;
in vec3 click_point;

out vec4 outColor;

void main(){
    // vec3 click_point = vec3(0.0, 0.1, 2.0);

    vec3 this_to_click = world_position - click_point;
    float distance_to_click = length(this_to_click);

    float intensity = (1.0 / pow(distance_to_click, 2));

    outColor = vec4(0.0, 1.0 * intensity, intensity, 0.5);
    // outColor = vec4(world_position.x, 0.0, 0.0, 1.0);
}
