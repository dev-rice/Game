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
    void addTexture(TextureLayer layer);

    void updateUniforms(GLuint);
    void setTextureLocations(GLuint);

private:

    std::vector<GLuint> unique_splatmaps;
    std::vector<TextureLayer> texture_layers;

    int current_index = 0;

};

#endif
