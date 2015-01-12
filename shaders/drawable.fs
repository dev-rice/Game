#version 330

in float distance_to_click;
in vec3 world_position;
out vec4 outColor;

void main(){
    // outColor = vec4(0.0, 1.0, 0.83, 0.5);
    outColor = vec4(world_position.x, 0.0, 0.0, 1.0);
}
