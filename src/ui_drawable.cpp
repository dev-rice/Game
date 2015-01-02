#include "ui_drawable.h"

UIDrawable::UIDrawable(FlatMesh* flat_mesh, Window* window, GLuint shader_program, GLuint texture) : FlatDrawable(flat_mesh, shader_program){
    int window_width = window->getWidth();
    int window_height = window->getHeight();

    mesh_projection = glm::mat3( window_width, 0.0f  , 0.0f,
                                      0.0f , window_height, 0.0f,
                                      0.0f , 0.0f  , 1.0f );
    attachTexture(texture);

}

void UIDrawable::attachTexture(GLuint texture){
    glBindTexture(GL_TEXTURE_2D, texture);
    int texture_width, texture_height;
    int miplevel = 0;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &texture_width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &texture_height);

    glm::vec3 image_size = glm::vec3(texture_width, texture_height, 1.0);
    glm::vec3 gl_mesh_size = image_size * glm::inverse(mesh_projection);

    width = gl_mesh_size.x;
    height = gl_mesh_size.y;

    FlatDrawable::attachTexture(texture);
}

void UIDrawable::setPosition(glm::vec2 position){
    this->position = glm::vec2(position.x + width, position.y - height);
}
