#include "texture_painter.h"

TexturePainter::TexturePainter() : TexturePainter(0) {}

TexturePainter::TexturePainter(GLuint texture){
    brush.bitmap = new GLubyte[16];
    for (int i = 0; i < 16; ++i){
        if (i % 4 == 0){
            brush.bitmap[i] = 255;
        } else {
            brush.bitmap[i] = 0;
        }
    }

    setTexture(texture);
}

GLuint TexturePainter::getTexture(){
    return texture;
}

void TexturePainter::setTexture(GLuint texture){
    this->texture = texture;
    texture_bytes = TextureLoader::getBytesFromTexture(texture);
}

char TexturePainter::getChannel(){
    return channel;
}

void TexturePainter::setChannel(char channel){
    this->channel = channel;
}

void TexturePainter::paint(int x, int y, Brush::Mode mode){
    int width = TextureLoader::getTextureWidth(texture);
    int height = TextureLoader::getTextureHeight(texture);

    int channel_int = TextureLayer::getIntFromChannelChar(channel);
    // for(int i = 0; i < 4 * width * height; ++i){
    //     if (i % 4 == (channel_int - 1)){
    //         texture_bytes[i] = 255;
    //     }
    // }
    GLubyte value = 0;
    if (mode == Brush::Mode::ERASE){
        value = 0;
    } else if (mode == Brush::Mode::PAINT){
        value = 255;
    }

    int index = getIndex(x, y, width);
    texture_bytes[index + channel_int - 1] = value;

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, (GLvoid*)texture_bytes);
}

int TexturePainter::getIndex(int x, int y, int width){
    return 4 * (x + ((width) * y));
}
