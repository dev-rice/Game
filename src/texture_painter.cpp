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

void TexturePainter::setTexture(GLuint texture){
    this->texture = texture;
    texture_bytes = TextureLoader::getBytesFromTexture(texture);

}

void TexturePainter::paint(int x, int y){
    int width = TextureLoader::getTextureWidth(texture);
    int height = TextureLoader::getTextureHeight(texture);

    for(int i = 0; i < 4 * width * height; ++i){
        if (i % 4 == 0){
            texture_bytes[i] = 255;
        }
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, (GLvoid*)texture_bytes);
}
