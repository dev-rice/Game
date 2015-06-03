#ifndef LayeredTextures_h
#define LayeredTextures_h

#include "includes/gl.hpp"
#include "includes/glm.hpp"

#include <vector>

#include "texture_layer.h"
#include "shader.hpp"

class LayeredTextures {
public:
    LayeredTextures(int);

    void addSplatmap(Texture splatmap);
    void addTexture(Texture diffuse, GLuint splatmap, char channel, int layer_number);

    void updateUniforms(Shader shader);
    void setTextureLocations(Shader shader);

    void swapLayers(GLuint layer1, GLuint layer2);

    bool needsSplatmaps();

    GLuint getSplatmap(int index);
    GLuint getTexture(GLuint splatmap, char channel);

    TextureLayer getLayer(GLuint splatmap, char channel);
    TextureLayer getLayer(GLuint layer_number);

    int getNumLayers();

    std::string saveData(std::string name);

private:

    std::vector<GLuint> unique_splatmaps;
    std::vector<TextureLayer> texture_layers;

    int num_layers;
    int num_splatmaps;

};

#endif
