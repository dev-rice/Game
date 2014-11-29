#include "texture_container.h"

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

    char* temp_filename = (char*)filename;

    std::string name = basename(temp_filename);
    textures[name] = texture;

    temp_filename = NULL;
    delete temp_filename;
}

GLuint TextureContainer::getTexture(std::string name){
    return textures[name];
}

void TextureContainer::print(){
    std::map<std::string, GLuint>::iterator it;
    for (it = textures.begin(); it != textures.end(); ++it){
        printf("%s => %d\n", (it->first).c_str(), it->second);
    }
    printf("\n");
}