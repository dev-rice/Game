#include "texture_loader.h"

GLuint TextureLoader::pink;
GLuint TextureLoader::alpha;
GLuint TextureLoader::black;
GLuint TextureLoader::blue;

bool TextureLoader::loaded_pink;
bool TextureLoader::loaded_alpha;
bool TextureLoader::loaded_black;
bool TextureLoader::loaded_blue;

GLuint TextureLoader::loadPink(){
    if (!loaded_pink){
        std::vector<GLfloat> pink_pixel;
        pink_pixel.push_back(1.0f);
        pink_pixel.push_back(0.0f);
        pink_pixel.push_back(1.0f);
        pink_pixel.push_back(1.0f);

        TextureLoader::pink = TextureLoader::loadTextureFromPixel(pink_pixel);
        loaded_pink = true;
    }
    return TextureLoader::pink;
}

GLuint TextureLoader::loadAlpha(){
    if (!loaded_alpha){
        std::vector<GLfloat> alpha_pixel;
        alpha_pixel.push_back(0.0f);
        alpha_pixel.push_back(0.0f);
        alpha_pixel.push_back(0.0f);
        alpha_pixel.push_back(0.0f);

        TextureLoader::alpha = TextureLoader::loadTextureFromPixel(alpha_pixel);
        loaded_alpha = true;
    }
    return TextureLoader::alpha;

}

GLuint TextureLoader::loadBlack(){
    if (!loaded_black){
        std::vector<GLfloat> black_pixel;
        black_pixel.push_back(0.0f);
        black_pixel.push_back(0.0f);
        black_pixel.push_back(0.0f);
        black_pixel.push_back(0.75f);

        TextureLoader::black = TextureLoader::loadTextureFromPixel(black_pixel);
        loaded_black = true;
    }
    return TextureLoader::black;

}

GLuint TextureLoader::loadBlue(){
    if (!loaded_blue){
        std::vector<GLfloat> blue_pixel;
        blue_pixel.push_back(0.5f);
        blue_pixel.push_back(0.5f);
        blue_pixel.push_back(1.0f);
        blue_pixel.push_back(1.0f);

        TextureLoader::blue = TextureLoader::loadTextureFromPixel(blue_pixel);
        loaded_blue = true;
    }
    return TextureLoader::blue;
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Do nearest interpolation for scaling the image up and down.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);
    // Mipmaps increase efficiency or something
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);

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

    return texture;
}
