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

#include "debug.h"

class TextureLoader {
public:
    static void loadDefaults();

    static GLuint loadPink();
    static GLuint loadAlpha();
    static GLuint loadGray();
    static GLuint loadBlue();

    static GLuint loadTextureFromFile(const char*, GLuint);
    static GLuint loadTextureFromPixel(std::vector<GLfloat>);

private:
    static GLuint pink;
    static GLuint alpha;
    static GLuint gray;
    static GLuint blue;

    static bool loaded_pink;
    static bool loaded_alpha;
    static bool loaded_gray;
    static bool loaded_blue;

};

#endif
