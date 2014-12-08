#ifndef TextureLoader_h
#define TextureLoader_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

#include <stdio.h>
#include <SOIL.h>
#include <vector>

class TextureLoader {
public:
    static void loadDefaults();

    static GLuint loadPink();
    static GLuint loadAlpha();

    // static GLuint alpha;
    // static Gluint pink;

    static GLuint loadTextureFromFile(const char*, GLuint);
    static GLuint loadTextureFromPixel(std::vector<GLfloat>);

};

#endif