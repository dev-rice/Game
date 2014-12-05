#ifndef TextureSet_h
#define TextureSet_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

struct TextureSet {
    GLuint diffuse;
    GLuint specular;
    GLuint normal;
    GLuint emissive;
};

#endif