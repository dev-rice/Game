#version 330 core

// Ouput data
out vec4 outColor;

void main(){
    float depth_projective = gl_FragCoord.z;

    float z_near = 0.1;
    float z_far = 500.0;
    float depth_linear = ((z_near * z_far) / (z_near - z_far)) / ((depth_projective - z_far) / (z_far - z_near));

    outColor = vec4(depth_projective * vec3(1, 1, 1), 1);
}
