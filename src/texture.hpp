#ifndef Texture_h
#define Texture_h

#include <string>

#include "includes/gl.hpp"
#include "includes/glm.hpp"
#include "includes/soil.hpp"
#include "debug.h"
#include "file.hpp"

using namespace std;

class Texture : public File {
public:
    Texture();
    Texture(GLuint id);
    Texture(GLubyte* data, GLuint width, GLuint height);
    Texture(glm::vec4 color);
    Texture(glm::vec4 color, GLuint width, GLuint height);
    Texture(string filename);

    void saveAs(string filename);
    string asJsonString(string type);

    GLuint getWidth();
    GLuint getHeight();
    GLubyte* getBytes();
    GLuint getGLId();

    GLuint getFormat();
    void setFormat(GLuint format);

private:
    GLuint loadTextureFromBytes(GLubyte* data, GLuint width, GLuint height, GLuint filter);

    GLuint loadTextureFromFile(std::string, GLuint);

    GLuint loadTextureFromPixel(glm::vec4 pixel, GLuint format);
    GLuint loadTextureFromPixel(GLuint width, GLuint height, glm::vec4 pixel, GLuint format);

    void saveTextureBytesToFile(GLubyte* data, GLuint width, GLuint height, GLuint channels, std::string filename);

    GLuint gl_texture_id;

    GLuint format;

};

#endif
