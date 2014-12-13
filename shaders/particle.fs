#version 330

in vec2 Texcoord;

out vec4 outColor;

uniform float time;
uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_map;
uniform sampler2D emissive_texture;

void main(){
    outColor = texture(emissive_texture, Texcoord);
}