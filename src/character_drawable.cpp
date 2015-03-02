#include "character_drawable.h"

CharacterDrawable::CharacterDrawable(GLuint shader_program, FontSheet* font_sheet, GLint point)
    :UIDrawable(shader_program, font_sheet->getTexture()) {
        mesh = CharacterMesh::getInstance();
        this->mesh->attachGeometryToShader(shader_program);
        uv_offset = glm::vec2();

        height_pixels = point;
        width_pixels = point;
        spacing = 1;

        setTextColor(1.0, 1.0, 1.0);

        updateDimensions();
        setGLPosition(getGLPosition());

        this->font_sheet = font_sheet;

}

void CharacterDrawable::setPixelPosition(int x, int y){
    x_pixels = x;
    y_pixels = y;
    base_x_pixels = x;
    base_y_pixels = y;

    setGLPosition(getGLPosition());
}

void CharacterDrawable::setTextColor(glm::vec3 color){
    text_color = color;
}

void CharacterDrawable::setTextColor(float r, float g, float b){
    setTextColor(glm::vec3(r, g, b));
}

void CharacterDrawable::setSpacing(float spacing){
    this->spacing = spacing;
}

void CharacterDrawable::moveToNext(){
    base_x_pixels += current_glyph.advance * spacing;
    setGLPosition(getGLPosition());

}

void CharacterDrawable::setCharacter(char to_render){
    current_glyph = font_sheet->getGlyph(to_render);

    uv_offset = glm::vec2(current_glyph.u_offset, current_glyph.v_offset);

    x_pixels = base_x_pixels + current_glyph.bearing_x;
    y_pixels = base_y_pixels - current_glyph.bearing_y + font_sheet->getPointSize();

    setGLPosition(getGLPosition());

}

void CharacterDrawable::updateUniformData(){
    glUniform2f(glGetUniformLocation(shader_program, "uv_offset"), uv_offset.x, uv_offset.y);
    glUniform3f(glGetUniformLocation(shader_program, "textColor"), text_color.x, text_color.y, text_color.z);
}
