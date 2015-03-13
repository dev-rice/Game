#include "texture_painter.h"

TexturePainter::TexturePainter() : TexturePainter(0) {}

TexturePainter::TexturePainter(GLuint texture){
    GLuint brush_texture = TextureLoader::loadTextureFromFile("res/textures/test_brush.png", GL_LINEAR);
    brush.bitmap = TextureLoader::getBytesFromTexture(brush_texture, GL_RED);
    brush.width = TextureLoader::getTextureWidth(brush_texture);
    brush.height = TextureLoader::getTextureHeight(brush_texture);

    setTexture(texture);
}

GLuint TexturePainter::getTexture(){
    return texture;
}

void TexturePainter::setTexture(GLuint texture){
    this->texture = texture;
    texture_bytes = TextureLoader::getBytesFromTexture(texture, GL_RGBA);
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

    Debug::info("width = %d\n", width);
    Debug::info("height = %d\n", height);

    int channel_int = TextureLayer::getIntFromChannelChar(channel);

    int upper_left_x = x - (brush.width / 2);
    int upper_left_y = y - (brush.height / 2);

    int lower_right_x = x + (brush.width / 2);
    int lower_right_y = y + (brush.height / 2);

    if (brush.width % 2 == 0){
        upper_left_x += 1;
    }
    if (brush.height % 2 == 0){
        upper_left_y += 1;
    }

    int brush_index = 0;
    for (int brush_x = upper_left_x; brush_x <= lower_right_x; ++brush_x){
        for (int brush_y = upper_left_y; brush_y <= lower_right_y; ++brush_y){
            int value = brush.bitmap[brush_index];
            brush_index++;

            int index = getIndex(brush_x, brush_y, width);
            int new_value = 0;
            if (mode == Brush::Mode::PAINT){
                new_value = value + texture_bytes[index + channel_int - 1];
            } else if (mode == Brush::Mode::ERASE){
                new_value = texture_bytes[index + channel_int - 1] - value;
            }

            // Clamp the value to a byte size. This prevents the byte from
            // overflowing.
            new_value = std::max(0, new_value);
            new_value = std::min(255, new_value);
            texture_bytes[index + channel_int - 1] = new_value;
        }
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, (GLvoid*)texture_bytes);
}

int TexturePainter::getIndex(int x, int y, int width){
    return 4 * (x + ((width) * y));
}
