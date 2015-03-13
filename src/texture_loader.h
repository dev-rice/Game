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

    static GLuint loadTextureFromBytes(GLubyte* data, GLuint width, GLuint height, GLuint filter);

    static GLuint loadTextureFromFile(std::string, GLuint);

    static GLuint loadTextureFromPixel(glm::vec4 pixel);
    static GLuint loadTextureFromPixel(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    static GLuint loadTextureFromPixel(std::string id, glm::vec4 pixel);
    static GLuint loadTextureFromPixel(std::string id, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

    static GLuint loadTextureFromPixel(GLuint width, GLuint height, glm::vec4 pixel);
    static GLuint loadTextureFromPixel(GLuint width, GLuint height, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    static GLuint loadTextureFromPixel(std::string id, GLuint width, GLuint height, glm::vec4 pixel);
    static GLuint loadTextureFromPixel(std::string id, GLuint width, GLuint height, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

    static GLubyte* getBytesFromTexture(GLuint texture_id, GLuint format);
    static GLuint getTextureWidth(GLuint texture_id);
    static GLuint getTextureHeight(GLuint texture_id);

    static void saveTextureToFile(GLuint texture_id, GLuint format, std::string filename);

private:

    static std::map<std::string, GLuint> loaded_textures;

};

#endif
