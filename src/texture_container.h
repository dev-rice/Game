#ifndef TextureContainer_h
#define TextureContainer_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

#include <SOIL.h>

#include <vector>
#include <string>
#include <libgen.h>
#include <stdlib.h>

class TextureContainer {
public:
    TextureContainer() {;}
    void addTexture(const char*, GLuint);
    GLuint getTexture(int);

private:
    std::vector<GLuint> textures;
};

#endif