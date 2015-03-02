#include "character_drawable.h"

CharacterDrawable::CharacterDrawable(GLuint shader_program, GLuint texture, GLint point)
    :UIDrawable(shader_program, texture) {
        mesh = CharacterMesh::getInstance();
        this->mesh->attachGeometryToShader(shader_program);
        uv_offset = glm::vec2();

        height_pixels = point;
        width_pixels = point * (CharacterMesh::CHARACTER_WIDTH / CharacterMesh::CHARACTER_HEIGHT);
        spacing = 0;

        setTextColor(1.0, 1.0, 1.0);

        updateDimensions();
        setGLPosition(getGLPosition());
}

void CharacterDrawable::setPixelPosition(int x, int y){
    x_pixels = x;
    y_pixels = y;

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
    x_pixels += width_pixels + spacing - CharacterMesh::PADDING;
    setGLPosition(getGLPosition());

}

void CharacterDrawable::setCharacter(char to_render){
    int ascii = to_render;
    int row = 0;
    int column = ascii + 1;

    float delta_u = CharacterMesh::CHARACTER_WIDTH / CharacterMesh::IMAGE_WIDTH;
    float delta_v = CharacterMesh::CHARACTER_HEIGHT / CharacterMesh::IMAGE_HEIGHT;

    uv_offset.x = column * delta_u;
    uv_offset.y = ((row) * delta_v);

}

void CharacterDrawable::updateUniformData(){
    glUniform2f(glGetUniformLocation(shader_program, "uv_offset"), uv_offset.x, uv_offset.y);
    glUniform3f(glGetUniformLocation(shader_program, "textColor"), text_color.x, text_color.y, text_color.z);
}
