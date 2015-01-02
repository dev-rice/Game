#ifndef TextureSet_h
#define TextureSet_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

#include <vector>

#include "texture_loader.h"

class TextureSet {
public:
    TextureSet();
    TextureSet(GLuint, GLuint, GLuint, GLuint);
  
    GLuint getDiffuse();
    GLuint getSpecular();
    GLuint getNormal();
    GLuint getEmissive();

    void load(GLuint, GLuint, GLuint, GLuint);

private:

    std::vector<GLuint> diffuse_set;
    std::vector<GLuint> specular_set;
    std::vector<GLuint> normal_set;
    std::vector<GLuint> emissive_set;  
};

#endif