#version 330 core

// Ouput data
out vec4 outColor;

void main(){
    // Not really needed, OpenGL does it anyway
    outColor = vec4(vec3(1, 1, 1) * gl_FragCoord.z, 1);
}
