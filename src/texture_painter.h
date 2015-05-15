#ifndef TexturePainter_h
#define TexturePainter_h

#include "includes/gl.hpp"
#include "includes/glm.hpp"

#include <random>

#include "debug.h"
#include "texture_loader.h"
#include "texture_layer.h"

// Temporary
struct Brush{
    GLubyte* bitmap;
    int width, height;
    enum Mode {PAINT, ERASE};
};

class TexturePainter {
public:
    TexturePainter();
    TexturePainter(GLuint texture);

    GLuint getTexture();
    void setTexture(GLuint texture);

    char getChannel();
    void setChannel(char channel);

    void paint(int x, int y, Brush::Mode mode);

private:

    int getIndex(int x, int y, int width);

    GLuint texture;
    GLubyte* texture_bytes;

    char channel;

    Brush brush;

};

#endif
