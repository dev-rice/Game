#version 150

in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D base_texture;
uniform float opacity;

void main() {
    const int BLUR_SIZE = 17;
    const int NUM_TEXELS = BLUR_SIZE / 2 + 1;
    vec4 texels[NUM_TEXELS];

    // Linear sample optimization from
    // http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/
    texels[0] = textureOffset(base_texture, Texcoord, ivec2(-7.5, 0));
    texels[1] = textureOffset(base_texture, Texcoord, ivec2(-5.5, 0));
    texels[2] = textureOffset(base_texture, Texcoord, ivec2(-3.5, 0));
    texels[3] = textureOffset(base_texture, Texcoord, ivec2(-1.5, 0));
    texels[4] = textureOffset(base_texture, Texcoord, ivec2(0, 0));
    texels[5] = textureOffset(base_texture, Texcoord, ivec2(1.5, 0));
    texels[6] = textureOffset(base_texture, Texcoord, ivec2(3.5, 0));
    texels[7] = textureOffset(base_texture, Texcoord, ivec2(5.5, 0));
    texels[8] = textureOffset(base_texture, Texcoord, ivec2(7.5, 0));

    vec4 total = texels[0] + texels[1] + texels[2] + texels[3] + texels[4] +
        texels[5] + texels[6] + texels[7] + texels[8];

    vec4 average_texel = total / NUM_TEXELS;

    outColor = vec4(average_texel.rgb, opacity * average_texel.a);
}
