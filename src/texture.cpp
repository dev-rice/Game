#include "texture.hpp"

Texture::Texture() {
    gl_texture_id = 0;
}

Texture::Texture(GLuint id) {
    gl_texture_id = id;
}

Texture::Texture(GLubyte* data, GLuint width, GLuint height) {

    gl_texture_id = loadTextureFromBytes(data, width, height, GL_LINEAR);

}

Texture::Texture(glm::vec4 color) {
    gl_texture_id = loadTextureFromPixel(color, GL_NEAREST);
}

Texture::Texture(glm::vec4 color, GLuint width, GLuint height) {

    gl_texture_id = loadTextureFromPixel(width, height, color, GL_LINEAR);

}

Texture::Texture(string filename) : filename(filename) {
    gl_texture_id = loadTextureFromFile(filename, GL_LINEAR);
}

string Texture::asJsonString(string type) {
    // Returns the texture as a json string
    // The type is diff, spec, norm, emit, etc...
    // Example:
    //      "diff": "fence_diff.png",

    string json_string = "\"" + type + "\": \"" + filename + "\",\n";

    // If there is no filename, then this is not a "saveable" texture.
    if (filename == "") {
        json_string = "";
    }


    return json_string;
}

GLuint Texture::getWidth(){
    int width;
    int miplevel = 0;
    glBindTexture(GL_TEXTURE_2D, gl_texture_id);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &width);
    return width;
}

GLuint Texture::getHeight(){
    int height;
    int miplevel = 0;
    glBindTexture(GL_TEXTURE_2D, gl_texture_id);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &height);
    return height;
}

GLubyte* Texture::getBytes(GLuint format){
    int width = getWidth();
    int height = getHeight();

    GLubyte* image = new GLubyte[4 * width * height];

    glBindTexture(GL_TEXTURE_2D, gl_texture_id);
    glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, image);

    return image;
}

GLuint Texture::getGLId() {
    return gl_texture_id;
}

void Texture::save(GLuint format, string filename){
    int channels = 0;
    switch(format){
        case GL_RED:
            channels = 1;
            break;
        case GL_RGB:
            channels = 3;
            break;
        case GL_RGBA:
            channels = 4;
            break;
        default:
            Debug::error("Image format %d is not supported.\n");
            return;
            break;

    }
    int width = getWidth();
    int height = getHeight();

    GLubyte* image = new GLubyte[channels * width * height];

    glBindTexture(GL_TEXTURE_2D, gl_texture_id);
    glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, image);

    saveTextureBytesToFile(image, width, height, channels, filename);

    delete[] image;
    image = NULL;
}

GLuint Texture::loadTextureFromBytes(GLubyte* data, GLuint width, GLuint height, GLuint filter){
    GLuint texture;
    // Set the active texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);

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

    return texture;
}

GLuint Texture::loadTextureFromFile(string filename, GLuint filter){
    GLuint texture;

    string id = filename + std::to_string(filter);

    // Load the image
    int width, height;
    GLubyte* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);

    texture = loadTextureFromBytes(image, width, height, filter);
    SOIL_free_image_data(image);

    return texture;

}

GLuint Texture::loadTextureFromPixel(glm::vec4 pixel, GLuint format){
    GLuint texture;

    GLubyte* data = new GLubyte[4];
    data[0] = pixel.x * 255;
    data[1] = pixel.y * 255;
    data[2] = pixel.z * 255;
    data[3] = pixel.w * 255;

    texture = loadTextureFromBytes(data, 1, 1, format);

    return texture;
}

GLuint Texture::loadTextureFromPixel(GLuint width, GLuint height, glm::vec4 pixel, GLuint format){

    GLuint texture;

    GLubyte* data = new GLubyte[4*width*height];
    for (int i = 0; i < 4 * width * height; i += 4){
        data[i] = pixel.x * 255;
        data[i + 1] = pixel.y * 255;
        data[i + 2] = pixel.z * 255;
        data[i + 3] = pixel.w * 255;
    }

    texture = loadTextureFromBytes(data, width, height, format);

    return texture;
}

void Texture::saveTextureBytesToFile(GLubyte* data, GLuint width, GLuint height, GLuint channels, string filename){
    int save_result = SOIL_save_image(filename.c_str(), SOIL_SAVE_TYPE_BMP, width, height, channels, data);
    if (!save_result){
        Debug::error("Error saving %s.\n", filename.c_str());
    }
}
