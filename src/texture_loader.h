#ifndef TextureLoader_h
#define TextureLoader_h

#include <GL/glew.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <SOIL.h>
#include <vector>
#include <string>
#include <map>

#include "debug.h"

class TextureLoader {
public:
    static void loadDefaults();

    static GLuint loadPink();
    static GLuint loadAlpha();
    static GLuint loadBlack();
    static GLuint loadBlue();
    static GLuint loadGreen();
    static GLuint loadRed();

    static GLuint loadTextureFromFile(std::string, GLuint);
    static GLuint loadTextureFromPixel(std::string id, glm::vec4 pixel);

private:

    static std::map<std::string, GLuint> loaded_textures;

};

#endif
