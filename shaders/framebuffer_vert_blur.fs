#version 150

in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D base_texture;
uniform float opacity;

void main() {
    const int BLUR_SIZE = 17;
    vec4 texels[BLUR_SIZE];

    texels[0] = textureOffset(base_texture, Texcoord, ivec2(0, -8));
    texels[1] = textureOffset(base_texture, Texcoord, ivec2(0, -7));
    texels[2] = textureOffset(base_texture, Texcoord, ivec2(0, -6));
    texels[3] = textureOffset(base_texture, Texcoord, ivec2(0, -5));
    texels[4] = textureOffset(base_texture, Texcoord, ivec2(0, -4));
    texels[5] = textureOffset(base_texture, Texcoord, ivec2(0, -3));
    texels[6] = textureOffset(base_texture, Texcoord, ivec2(0, -2));
    texels[7] = textureOffset(base_texture, Texcoord, ivec2(0, -1));
    texels[8] = textureOffset(base_texture, Texcoord, ivec2(0, 0));
    texels[9] = textureOffset(base_texture, Texcoord, ivec2(0, 1));
    texels[10] = textureOffset(base_texture, Texcoord, ivec2(0, 2));
    texels[11] = textureOffset(base_texture, Texcoord, ivec2(0, 3));
    texels[12] = textureOffset(base_texture, Texcoord, ivec2(0, 4));
    texels[13] = textureOffset(base_texture, Texcoord, ivec2(0, 5));
    texels[14] = textureOffset(base_texture, Texcoord, ivec2(0, 6));
    texels[15] = textureOffset(base_texture, Texcoord, ivec2(0, 7));
    texels[16] = textureOffset(base_texture, Texcoord, ivec2(0, 8));

    vec4 total = texels[0] + texels[1] + texels[2] + texels[3] + texels[4] +
        texels[5] + texels[6] + texels[7] + texels[8] + texels[9] +
        texels[10] + texels[11] + texels[12] + texels[13] + texels[14] +
        texels[15] + texels[16];

    vec4 average_texel = total / BLUR_SIZE;

    outColor = vec4(average_texel.rgb, opacity * average_texel.a);
}
