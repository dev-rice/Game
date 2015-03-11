#include "texture_painter.h"

TexturePainter::TexturePainter() : TexturePainter(0) {}

TexturePainter::TexturePainter(GLuint texture){
    brush.bitmap = new GLubyte[16];
    for (int i = 0; i < 16; ++i){
        brush.bitmap[i] = 255;
    }
    brush.width = 4;
    brush.height = 4;

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

    int upper_left_x = x - (brush.width / 2);
    int upper_left_y = y - (brush.height / 2);

    int lower_right_x = x + (brush.width / 2);
    int lower_right_y = y + (brush.height / 2);

    if (brush.width % 2 == 0){
        upper_left_x -= 1;
    }
    if (brush.height % 2 == 0){
        upper_left_y -= 1;
    }

    for (int brush_x = upper_left_x; brush_x < lower_right_x; ++brush_x){
        for (int brush_y = upper_left_y; brush_y < lower_right_y; ++brush_y){
            GLubyte value = 0;
            if (mode == Brush::Mode::ERASE){
                value = 0;
            } else if (mode == Brush::Mode::PAINT){
                value = 255;
            }

            int index = getIndex(brush_x, brush_y, width);
            texture_bytes[index + channel_int - 1] = value;
        }
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, (GLvoid*)texture_bytes);
}

int TexturePainter::getIndex(int x, int y, int width){
    return 4 * (x + ((width) * y));
}
