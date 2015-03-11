#include "texture_loader.h"

std::map<std::string, GLuint> TextureLoader::loaded_textures;

GLuint TextureLoader::loadPink(){
    glm::vec4 pink_pixel = glm::vec4(1.0f, 0.0f, 1.0f, 1.00f);
    return TextureLoader::loadTextureFromPixel("pink", pink_pixel);
}

GLuint TextureLoader::loadAlpha(){
    glm::vec4 alpha_pixel = glm::vec4(0.0f, 0.0f, 0.0f, 0.00f);
    return TextureLoader::loadTextureFromPixel("alpha", alpha_pixel);
}

GLuint TextureLoader::loadBlack(){
    glm::vec4 black_pixel = glm::vec4(0.0f, 0.0f, 0.0f, 0.75f);
    return TextureLoader::loadTextureFromPixel("black", black_pixel);
}

GLuint TextureLoader::loadBlue(){
    glm::vec4 blue_pixel = glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);
    return TextureLoader::loadTextureFromPixel("blue", blue_pixel);
}

GLuint TextureLoader::loadGreen(){
    glm::vec4 green_pixel = glm::vec4(0.172f, 0.855f, 0.424f, 1.0f);
    return TextureLoader::loadTextureFromPixel("green", green_pixel);
}

GLuint TextureLoader::loadRed(){
    glm::vec4 red_pixel = glm::vec4(0.812f, 0.267f, 0.267f, 1.0f);
    return TextureLoader::loadTextureFromPixel("red", red_pixel);
}

GLuint TextureLoader::loadTextureFromFile(std::string filename, GLuint filter){
    GLuint texture;

    std::string id = filename + std::to_string(filter);

    if (loaded_textures.find(id) != loaded_textures.end()){
        texture = loaded_textures[id];
    } else{
        glGenTextures(1, &texture);

        // Load the texture
        int width, height;
        unsigned char* image;
        // Set the active texture
        glBindTexture(GL_TEXTURE_2D, texture);
        // Load the image
        image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
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
        TextureLoader::loaded_textures[id] = texture;
    }

    return texture;

}

GLuint TextureLoader::loadTextureFromPixel(glm::vec4 pixel){
    // Generate id using the color values
    std::string id = "";
    id += std::to_string(pixel.x);
    id += std::to_string(pixel.y);
    id += std::to_string(pixel.z);
    id += std::to_string(pixel.w);

    // Load the texture using the generated id
    return loadTextureFromPixel(id, pixel);
}

GLuint TextureLoader::loadTextureFromPixel(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha){
    glm::vec4 pixel = glm::vec4(red, green, blue, alpha);
    return loadTextureFromPixel(pixel);
}

GLuint TextureLoader::loadTextureFromPixel(std::string id, glm::vec4 pixel){
    GLuint texture;

    if (loaded_textures.find(id) != loaded_textures.end()){
        texture = loaded_textures[id];
    } else {
        glGenTextures(1, &texture);

        // Set the active texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // Load the image
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA,
                     GL_FLOAT, glm::value_ptr(pixel));
        // Set the texture wrapping to repeat
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // Do nearest interpolation for scaling the image up and down.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        TextureLoader::loaded_textures[id] = texture;
    }

    return texture;
}

GLuint TextureLoader::loadTextureFromPixel(std::string id, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha){
    glm::vec4 pixel = glm::vec4(red, green, blue, alpha);
    return loadTextureFromPixel(id, pixel);
}

GLubyte* TextureLoader::getBytesFromTexture(GLuint texture_id, GLuint format){
    int width = getTextureWidth(texture_id);
    int height = getTextureHeight(texture_id);

    GLubyte* image = new GLubyte[4 * width * height];

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, image);

    return image;
}

GLuint TextureLoader::getTextureWidth(GLuint texture_id){
    int width;
    int miplevel = 0;
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &width);
    return width;
}

GLuint TextureLoader::getTextureHeight(GLuint texture_id){
    int height;
    int miplevel = 0;
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &height);
    return height;
}
