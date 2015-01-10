#include "character_drawable.h"

CharacterDrawable::CharacterDrawable(GLuint shader_program, GLuint texture, GLint point)
    :UIDrawable(shader_program, texture) {
        mesh = CharacterMesh::getInstance();
        this->mesh->attachGeometryToShader(shader_program);
        uv_offset = glm::vec2();

        height_pixels = point;
        width_pixels = point * (CharacterMesh::CHARACTER_WIDTH / CharacterMesh::CHARACTER_HEIGHT);

        Debug::info("Character dimensions for %d pt font = %d by %d.\n", point,
            height_pixels, width_pixels);

        updateDimensions();
        setGLPosition(getGLPosition());
}

void CharacterDrawable::setPixelPosition(int x, int y){
    x_pixels = x;
    y_pixels = y;

    setGLPosition(getGLPosition());
}

void CharacterDrawable::moveToNext(){
    x_pixels += width_pixels - 4;
    setGLPosition(getGLPosition());

}

void CharacterDrawable::setCharacter(char to_render){
    int ascii = to_render;
    int row = floor(((float)ascii - 32) / 16.0);
    int column = (ascii - 32) - (row * 16);

    float delta_u = CharacterMesh::CHARACTER_WIDTH / CharacterMesh::IMAGE_WIDTH;
    float delta_v = CharacterMesh::CHARACTER_HEIGHT / CharacterMesh::IMAGE_HEIGHT;

    uv_offset.x = column * delta_u;
    uv_offset.y = ((row) * delta_v);

}

void CharacterDrawable::updateUniformData(){
    glUniform2f(glGetUniformLocation(shader_program, "uv_offset"), uv_offset.x, uv_offset.y);
}
