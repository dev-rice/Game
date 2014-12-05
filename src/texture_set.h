#ifndef TextureSet_h
#define TextureSet_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

class TextureSet {
public:
    TextureSet() {;}
    TextureSet(GLint, GLint, GLint, GLint);

    GLuint diffuse;
    GLuint specular;
    GLuint normal;
    GLuint emissive;
};

#endif