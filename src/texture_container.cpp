#include "texture_container.h"

TextureContainer::TextureContainer(){
    // Sets the default texture to be pink, 100% alpha
    glGenTextures(1, &default_texture);

    // Set the active texture
    glBindTexture(GL_TEXTURE_2D, default_texture);
    
    // Load the image
    GLfloat pixel[] = {1.0f, 0.0f, 1.0f, 0.0f};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA,
                 GL_FLOAT, pixel);
    // Set the texture wrapping to repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Do nearest interpolation for scaling the image up and down.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void TextureContainer::addTexture(const char* filename, GLuint filter){
    GLuint texture;
    glGenTextures(1, &texture);

    // Load the texture
    int width, height;
    unsigned char* image;
    // Set the active texture
    glBindTexture(GL_TEXTURE_2D, texture);
    // Load the image
    image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, image);
    // Set the texture wrapping to repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Do nearest interpolation for scaling the image up and down.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    // Mipmaps increase efficiency or something
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);

    textures.push_back(texture);

    printf("Bound texture file '%s' to %d on the graphics card.\n", filename, texture);

}

GLuint TextureContainer::getTexture(int index){
    if (index == -1){
        return default_texture;
    } else {
        return textures[index];
    }
}