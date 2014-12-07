#include "texture_loader.h"

GLuint TextureLoader::loadPink(){
    std::vector<GLfloat> pink_pixel;
    pink_pixel.push_back(1.0f);
    pink_pixel.push_back(0.0f);
    pink_pixel.push_back(1.0f);
    pink_pixel.push_back(1.0f);
    return TextureLoader::loadTextureFromPixel(pink_pixel);
}

GLuint TextureLoader::loadAlpha(){
    std::vector<GLfloat> alpha_pixel;
    alpha_pixel.push_back(0.0f);
    alpha_pixel.push_back(0.0f);
    alpha_pixel.push_back(0.0f);
    alpha_pixel.push_back(0.0f);
    return TextureLoader::loadTextureFromPixel(alpha_pixel);
}

GLuint TextureLoader::loadTextureFromFile(const char* filename, GLuint filter){
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

    printf("loaded texture file '%s' to location %d\n", filename, texture);

    return texture;

}

GLuint TextureLoader::loadTextureFromPixel(std::vector<GLfloat> pixel){
    GLuint texture;
    
    // Sets the default texture to be pink, 100% alpha
    glGenTextures(1, &texture);

    // Set the active texture
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // Load the image
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA,
                 GL_FLOAT, pixel.data());
    // Set the texture wrapping to repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Do nearest interpolation for scaling the image up and down.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    printf("Loaded a texture from pixel and put it in location %d\n", texture);

    return texture;
}
