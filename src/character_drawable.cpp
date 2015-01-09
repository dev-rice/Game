#include "character_drawable.h"

CharacterDrawable::CharacterDrawable(GLuint shader_program, GLfloat width, GLfloat height, glm::vec2 position)
    :FlatDrawable(shader_program, width, height, position) {
        mesh = CharacterMesh::getInstance();
        this->mesh->attachGeometryToShader(shader_program);
        uv_offset = glm::vec2();
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
