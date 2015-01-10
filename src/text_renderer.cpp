#include "text_renderer.h"

TextRenderer::TextRenderer(std::string font_filename, GLint point){
    int width = Window::getInstance()->getWidth();
    int height = Window::getInstance()->getHeight();

    this->point = point;

    GLuint text_shader = ShaderLoader::loadShaderProgram("shaders/text.vs",
        "shaders/text.fs");

    GLuint character_texture = TextureLoader::loadTextureFromFile(font_filename.c_str(), GL_NEAREST);

    character_box = new CharacterDrawable(text_shader, character_texture, point);
    // character_box->attachTexture(character_texture);

}

void TextRenderer::drawString(int x, int y, std::string to_draw){
    float spacing = 0.1;
    character_box->setPixelPosition(x, y);
    for (int i = 0; i < to_draw.size(); ++i){
        character_box->setCharacter(to_draw[i]);
        character_box->moveToNext();
        character_box->draw();
    }
}

void TextRenderer::print(int x, int y, const char* format, ...){
    // Some real hacky C shit from
    // http://stackoverflow.com/questions/5876646/how-to-overload-printf-or-cout
    char buffer[256];

    va_list argument_list;
    va_start(argument_list, format);
    vsprintf(buffer, format, argument_list);
    va_end(argument_list);

    drawString(x, y, std::string(buffer));
}
