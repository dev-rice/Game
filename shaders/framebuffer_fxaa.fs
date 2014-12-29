// Implementation of algorithm described here:
// http://developer.download.nvidia.com/assets/gamedev/files/sdk/11/FXAA_WhitePaper.pdf

#version 330

in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D base_texture;

float FXAA_EDGE_THRESHOLD_MIN = 1.0 / 16.0;
float FXAA_EDGE_THRESHOLD = 1.0 / 8.0;

float FxaaLuma(vec4 color) {
    return color.g * (0.587/0.299) + color.r;
}

bool contrastCheck(vec4 current_pixel){
    float current_luminance = FxaaLuma(current_pixel);

    // Find the luminance of each neighboring pixel. (4x)
    vec4 north_pixel = textureOffset(base_texture, Texcoord, ivec2(0, 1));
    vec4 south_pixel = textureOffset(base_texture, Texcoord, ivec2(0, -1));
    vec4 east_pixel = textureOffset(base_texture, Texcoord, ivec2(1, 0));
    vec4 west_pixel = textureOffset(base_texture, Texcoord, ivec2(-1, 0));
    float north_luminance = FxaaLuma(north_pixel);
    float south_luminance = FxaaLuma(south_pixel);
    float east_luminance = FxaaLuma(east_pixel);
    float west_luminance = FxaaLuma(west_pixel);

    // Find the pixel with the smallest luminance
    float minimum_luminance = min(current_luminance, min(min(north_luminance, south_luminance),
    min(east_luminance, west_luminance)));

    // Find the pixel with the largest luminance
    float maximum_luminance = max(current_luminance, max(max(north_luminance, south_luminance),
    max(east_luminance, west_luminance)));

    // Find the range of luminance
    float range = maximum_luminance - minimum_luminance;

    // The threshold of the luminance range
    float threshold = max(FXAA_EDGE_THRESHOLD_MIN, maximum_luminance * FXAA_EDGE_THRESHOLD);

    return range > threshold;
}

void main() {
    vec4 current_pixel = texture(base_texture, Texcoord);
    bool is_contrast = contrastCheck(current_pixel);

    if (is_contrast){
        outColor = vec4(1.0, 0.0, 0.0, 1.0);
    } else {
        outColor = current_pixel;
    }
}
