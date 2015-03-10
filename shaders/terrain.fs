#version 330

struct Light {
    vec3 position;
    vec3 color;
    float power;

    vec3 light_to_surface;
};

struct Splat {
    // The splatmap to get data from
    sampler2D splatmap;

    // Actual textures to be drawn
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    sampler2D emissive;

    // Channel to get data from
    // 0 : red
    // 1 : green
    // 2 : blue
    int channel;

    // Layer number for the blending. Splats that are a higher layer number
    // will be painted on top of those with lower layer numbers.
    int layer;
};

const int num_lights = 3;

in vec2 Texcoord;
in vec2 Splatcoord;
in vec3 surface_normal;
in vec3 camera_to_surface;
in Light lights[num_lights];
in vec4 shadow_coord;

out vec4 outColor;

uniform float time;
uniform sampler2D specular_texture;
uniform sampler2D normal_map;
uniform sampler2D emissive_texture;
uniform sampler2D shadow_map;

uniform sampler2D splatmaps[2];
uniform sampler2D diffuse_textures[7];
uniform int channels[7];

uniform sampler2D splatmap_painted;
uniform sampler2D diffuse_painted;

uniform Splat splats[4];

vec4 diffuse;
vec4 specular;
vec4 normal;
vec4 emissive;

vec3 map_surface_normal;

layout(std140) uniform ProfileSettings {
    float lighting;
    float shadows;
    float normal_maps;
};

const bool NORMAL_DEBUG = false;

// Range: 0 to 4
// 0 is sharp
// 4 is pretty blurry
const float shadow_blurriness = 2.0;

// const int poisson_samples = 4;
// const vec2 poisson_disk[4] = vec2[]( vec2( -0.94201624, -0.39906216 ),
//                                      vec2( 0.94558609, -0.76890725 ),
//                                      vec2( -0.094184101, -0.92938870 ),
//                                      vec2( 0.34495938, 0.29387760 ) );

const int poisson_samples = 8;
const vec2 poisson_disk[8] = vec2[]( vec2(-0.1720364f, -0.2151852f),
                                     vec2(0.3053397f, 0.3449444f),
                                     vec2(0.345497f, -0.6371536f),
                                     vec2(-0.3129719f, -0.6846723f),
                                     vec2(-0.871155f, -0.4244208f),
                                     vec2(-0.564598f, -0.08805853f),
                                     vec2(0.4264781f, -0.07169557f),
                                     vec2(-0.3741353f, 0.45067f));

// const int poisson_samples = 16;
// const vec2 poisson_disk[16] = vec2[]( vec2(-0.1720364f, -0.2151852f),
//                                       vec2(0.3053397f, 0.3449444f),
//                                       vec2(0.345497f, -0.6371536f),
//                                       vec2(-0.3129719f, -0.6846723f),
//                                       vec2(-0.871155f, -0.4244208f),
//                                       vec2(-0.564598f, -0.08805853f),
//                                       vec2(0.4264781f, -0.07169557f),
//                                       vec2(-0.3741353f, 0.45067f),
//                                       vec2(0.8592183f, -0.1411734f),
//                                       vec2(0.5787006f, 0.731523f),
//                                       vec2(0.08467636f, 0.8623562f),
//                                       vec2(-0.7691356f, 0.2981086f),
//                                       vec2(-0.4271979f, 0.8866745f),
//                                       vec2(0.05698008f, -0.9206058f),
//                                       vec2(0.7499817f, -0.5811188f),
//                                       vec2(0.8284409f, 0.3923607f));


const int num_tiles = 128 / 16;
const vec2 tile_size_uv = vec2(16.0 / 128.0, 16.0 / 128.0);

float rand(vec4 seed){
    float dot_product = dot(seed, vec4(12.9898,78.233,45.164,94.673));
    return fract(sin(dot_product) * 43758.5453);
}

vec4 lightFragment(vec3 light_vector, vec3 light_color, float light_power){
    float intensity = light_power / (pow(light_vector.x, 2) + pow(light_vector.y,
         2) + pow(light_vector.z, 2));

    float cosTheta = dot(normalize(map_surface_normal), normalize(light_vector));
    cosTheta = clamp(cosTheta, 0.0, 1.0);

    vec3 reflection = reflect(-normalize(light_vector), normalize(map_surface_normal));
    float cosAlpha = clamp(dot(normalize(camera_to_surface), reflection),
        0.0, 1.0);


    vec4 diffuse_component = vec4(diffuse.rgb * intensity * cosTheta, diffuse.w);

    float specularity = specular.w;
    vec4 specular_component = vec4(specular.rgb * specularity * intensity *
        pow(cosAlpha,15), specular.w);

    vec4 lit_component = vec4(light_color, 1.0) *
        (diffuse_component + specular_component);
    return lit_component;
}

float getShadowFactor(){
    // Shadows
    float bias = 0.01;

    float angle = dot(normalize(map_surface_normal),
        normalize(lights[0].light_to_surface));

    bool is_back_face = angle < 0.2;
    is_back_face = false;
    bool in_shadow_map = (shadow_coord.x >= 0.0) && (shadow_coord.x <= 1.0) &&
        (shadow_coord.y >= 0.0) && (shadow_coord.y <= 1.0);

    float visibility = 1.0;
    for (int i = 0; i < poisson_samples; i++){
        vec2 poisson_coord = shadow_coord.xy + poisson_disk[i] * shadow_blurriness/700.0;
        float light_depth = texture(shadow_map, poisson_coord).r;
        float current_depth = shadow_coord.z - bias;

        if ((light_depth < current_depth) && in_shadow_map && !is_back_face){
            visibility -= 0.8 / poisson_samples;
        }
    }

    int shadow_sum = 0;
    // Definitely has some bugs
    // for (int i = -1; i < 2; ++i){
    //     for (int j = -1; j < 2; ++j){
    //         vec2 current_coord = shadow_coord.xy + vec2(i, j);
    //         float light_depth = texture(shadow_map, current_coord).z;
    //         float current_depth = shadow_coord.z - bias;
    //         if ((light_depth < current_depth) && in_shadow_map){
    //             shadow_sum++;
    //         }
    //     }
    // }

    // Debugging for the shadowmap box size
    if (!in_shadow_map){
        visibility = 0.2;
    }

    return visibility * (1 - (3.0 * shadow_sum / 9.0));
}

float getSplatValue(sampler2D splatmap, int channel){
    float splat_value = 0.0;
    if (channel == 1){
        splat_value = texture(splatmap, Splatcoord).r;
    } else if (channel == 2){
        splat_value = texture(splatmap, Splatcoord).g;
    } else if (channel == 3){
        splat_value = texture(splatmap, Splatcoord).b;
    } else {
        splat_value = texture(splatmap, Splatcoord).r;
    }
    return splat_value;
}

vec4 blendTexturesWithSplatmap(vec4 base_texture, sampler2D layer1, sampler2D layer2, sampler2D layer3, vec3 channel_vector, sampler2D splatmap){
    // Blends 4 textures together using the first argument as the base and
    // 0, 1, and 2 as the layers to paint on top.
    // Assumes that layer 1 has the red component, 2 has green, and 3 has blue
    vec4 blended = vec4(0.0, 0.0, 0.0, 1.0);

    // The splat values are encased in the splatmap as the red green and blue
    // channels. The first splat value is 1.0 because this is the base. The
    // others are defined by the channel vector.
    //
    // Channel vector:
    //      vec3(red_layer, green_layer, blue_layer)
    float splat_values[4];
    splat_values[0] = 1.0;
    splat_values[1] = getSplatValue(splatmap, int(channel_vector.x));
    splat_values[2] = getSplatValue(splatmap, int(channel_vector.y));
    splat_values[3] = getSplatValue(splatmap, int(channel_vector.z));

    vec4 layers[4];
    layers[0] = base_texture;
    layers[1] = texture(layer1, Texcoord);
    layers[2] = texture(layer2, Texcoord);
    layers[3] = texture(layer3, Texcoord);

    blended = mix(blended, layers[0], splat_values[0]);
    blended = mix(blended, layers[1], splat_values[1]);
    blended = mix(blended, layers[2], splat_values[2]);
    blended = mix(blended, layers[3], splat_values[3]);

    return blended;
}

void main() {
    vec3 channelmaps[2];
    channelmaps[0] = vec3(channels[1], channels[2], channels[3]);
    channelmaps[1] = vec3(channels[4], channels[5], channels[6]);

    vec4 base = texture(diffuse_textures[0], Texcoord);
    diffuse = blendTexturesWithSplatmap(base, diffuse_textures[1], diffuse_textures[2], diffuse_textures[3], channelmaps[0], splatmaps[0]);
    diffuse = blendTexturesWithSplatmap(diffuse, diffuse_textures[4], diffuse_textures[5], diffuse_textures[6], channelmaps[1], splatmaps[1]);

    specular = texture(specular_texture, Texcoord);
    emissive = texture(emissive_texture, Texcoord);

    bool lighting_on = lighting != 0.0f;
    bool shadows_on = shadows != 0.0f;
    bool normals_on = normal_maps != 0.0f;

    if (normals_on){
        map_surface_normal = (texture(normal_map, Texcoord) * 2 - vec4(1, 1, 1, 0)).rgb;
    } else {
        map_surface_normal = surface_normal;
    }

    float visibility;
    if (shadows_on){
        visibility = getShadowFactor();
    } else{
        visibility = 1.0;
    }

    vec4 texel;
    if (lighting_on){
        // Works fine
        Light light;
        vec4 lit_component = vec4(0.0, 0.0, 0.0, 0.0);

        // Direction light
        light = lights[0];
        lit_component = lit_component + visibility *
        lightFragment(light.light_to_surface, light.color, light.power);

        // Other lights
        light = lights[1];
        lit_component = lit_component +
        lightFragment(light.light_to_surface, light.color, light.power);
        light = lights[2];
        lit_component = lit_component +
        lightFragment(light.light_to_surface, light.color, light.power);

        vec4 ambient_component = vec4(0.05, 0.05, 0.05, 1.0) * diffuse;
        vec4 emissive_component = vec4(emissive.rgb, 1.0);
        texel = mix(lit_component + ambient_component, emissive_component,
            emissive.a);

    } else {
        texel = visibility * diffuse;
        texel.a = diffuse.a;

        vec4 emissive_component = vec4(emissive.rgb, 1.0);
        texel = mix(texel, emissive_component, emissive.a);
    }

    if (texel.a < 0.5){
        discard;
    }

    if (NORMAL_DEBUG){
        outColor = vec4(map_surface_normal, 1.0);
    } else {
        outColor = texel;
    }
}
