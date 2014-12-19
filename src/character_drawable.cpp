#include "character_drawable.h"

CharacterDrawable::CharacterDrawable(FlatMesh* mesh, GLuint shader_program, GLfloat width, GLfloat height, glm::vec2 position)
    :FlatDrawable(mesh, shader_program, width, height, position) {

        uv_offset = glm::vec2();
}

void CharacterDrawable::setCharacter(char to_render){
    int ascii = to_render;
    int row = floor(((float)ascii - 32) / 16.0);
    int column = (ascii - 32) - (row * 16);

    printf("%c is at (%d, %d).\n", to_render, row, column);

    float delta_u = CharacterMesh::CHARACTER_WIDTH / CharacterMesh::IMAGE_WIDTH;
    float delta_v = CharacterMesh::CHARACTER_HEIGHT / CharacterMesh::IMAGE_HEIGHT;

    printf("delta_u: %f\n", delta_u);
    printf("delta_v: %f\n", delta_v);

    uv_offset.x = column * delta_u;
    uv_offset.y = ((row) * delta_v);

    printf("%c is at (u, v) = (%f, %f).\n", to_render, uv_offset.x, uv_offset.y);

    // uv_offset.x = delta_u;
    // uv_offset.y = delta_v;    

    // printf("UV offset for character: (u, v) = (%f, %f).\n", uv_offset.x, uv_offset.y);


}

void CharacterDrawable::updateUniformData(){

    glUniform2f(glGetUniformLocation(shader_program, "uv_offset"), uv_offset.x, uv_offset.y);
}