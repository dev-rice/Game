#ifndef LayeredTextures_h
#define LayeredTextures_h

#include "includes/gl.hpp"
#include "includes/glm.hpp"

#include <vector>

#include "texture_layer.h"

class LayeredTextures {
public:
    LayeredTextures(int);

    void addSplatmap(GLuint splatmap);
    void addTexture(GLuint diffuse, GLuint splatmap, char channel, int layer_number);

    void updateUniforms(GLuint);
    void setTextureLocations(GLuint);

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
