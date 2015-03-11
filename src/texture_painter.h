//

#ifndef TexturePainter_h
#define TexturePainter_h

#include <GL/glew.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <random>

#include "debug.h"
#include "texture_loader.h"
#include "texture_layer.h"

// Temporary
struct Brush{
    GLubyte* bitmap;
};

class TexturePainter {
public:
    TexturePainter();
    TexturePainter(GLuint texture);

    GLuint getTexture();
    void setTexture(GLuint texture);

    char getChannel();
    void setChannel(char channel);

    void paint(int x, int y);

private:

    int getIndex(int x, int y, int width);

    GLuint texture;
    GLubyte* texture_bytes;

    char channel;

    Brush brush;

};

#endif
