#include "text_renderer.h"

TextRenderer::TextRenderer(std::string font_filename, GLfloat scale){
    int width = Window::getInstance()->getWidth();
    int height = Window::getInstance()->getHeight();

    this->scale = scale;

    GLuint text_shader = ShaderLoader::loadShaderProgram("shaders/text.vs",
        "shaders/text.fs");

    character_box = new CharacterDrawable(text_shader, scale,
        scale * ((float)width / (float)height), glm::vec2());

    GLuint character_texture = TextureLoader::loadTextureFromFile(font_filename.c_str(), GL_LINEAR);

    character_box->attachTexture(character_texture);

}

void TextRenderer::drawString(glm::vec2 position, std::string to_draw){
    float spacing = 0.1 * scale;
    for (int i = 0; i < to_draw.size(); ++i){
        character_box->setCharacter(to_draw[i]);
        character_box->setPosition(glm::vec2(position.x + (scale + spacing) * i,
            position.y));
        character_box->draw();
    }
}

void TextRenderer::print(glm::vec2 position, const char* format, ...){
    // Some real hacky C shit from
    // http://stackoverflow.com/questions/5876646/how-to-overload-printf-or-cout
    char buffer[256];

    va_list argument_list;
    va_start(argument_list, format);
    vsprintf(buffer, format, argument_list);
    va_end(argument_list);

    drawString(position, std::string(buffer));
}
