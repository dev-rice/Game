#ifndef LayeredTextures_h
#define LayeredTextures_h

#include <GL/glew.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    GLuint getSplatmap(int index);
    GLuint getTexture(GLuint splatmap, char channel);

    TextureLayer getLayer(GLuint splatmap, char channel);
    TextureLayer getLayer(GLuint layer_number);

private:

    std::vector<GLuint> unique_splatmaps;
    std::vector<TextureLayer> texture_layers;

    int num_layers;
    int num_splatmaps;

};

#endif
