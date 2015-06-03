#ifndef Texture_h
#define Texture_h

#include <string>

#include "includes/gl.hpp"
#include "includes/glm.hpp"
#include "includes/soil.hpp"
#include "debug.h"

using namespace std;

class Texture {
public:
    Texture();
    Texture(GLuint id);
    Texture(GLubyte* data, GLuint width, GLuint height);
    Texture(glm::vec4 color);
    Texture(glm::vec4 color, GLuint width, GLuint height);
    Texture(string filename);

    GLuint getWidth();
    GLuint getHeight();
    GLubyte* getBytes(GLuint format);

    GLuint getGLId();

private:
    GLuint loadTextureFromBytes(GLubyte* data, GLuint width, GLuint height, GLuint filter);

    GLuint loadTextureFromFile(std::string, GLuint);

    GLuint loadTextureFromPixel(glm::vec4 pixel);
    GLuint loadTextureFromPixel(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    GLuint loadTextureFromPixel(std::string id, glm::vec4 pixel);
    GLuint loadTextureFromPixel(std::string id, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

    GLuint loadTextureFromPixel(GLuint width, GLuint height, glm::vec4 pixel);
    GLuint loadTextureFromPixel(GLuint width, GLuint height, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    GLuint loadTextureFromPixel(std::string id, GLuint width, GLuint height, glm::vec4 pixel);
    GLuint loadTextureFromPixel(std::string id, GLuint width, GLuint height, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

    void saveTextureToFile(GLuint texture_id, GLuint format, std::string filename);
    void saveTextureBytesToFile(GLubyte* data, GLuint width, GLuint height, GLuint channels, std::string filename);


    GLuint gl_texture_id;

};

#endif
