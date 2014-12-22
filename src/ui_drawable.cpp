#include "ui_drawable.h"

UIDrawable::UIDrawable(FlatMesh* flat_mesh, GLFWwindow* window, GLuint shader_program, GLuint texture) : 
FlatDrawable(flat_mesh, shader_program){    
    int window_width, window_height;
    glfwGetFramebufferSize(window, &window_width, &window_height);

    glm::mat3 projection = glm::mat3( window_width, 0.0f  , 0.0f,
                                      0.0f , window_height, 0.0f,
                                      0.0f , 0.0f  , 1.0f );

    glBindTexture(GL_TEXTURE_2D, texture);
    int w, h;
    int miplevel = 0;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);

    glm::vec3 image_size = glm::vec3(w, h, 1.0);
    glm::vec3 gl_mesh_size = image_size * glm::inverse(projection);

    attachTexture(texture);

    width = gl_mesh_size.x;
    height = gl_mesh_size.y;

}

void UIDrawable::setPosition(glm::vec2 position){
    this->position = glm::vec2(position.x + width, position.y - height);
}
