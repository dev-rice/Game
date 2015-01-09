#include "ui_drawable.h"

UIDrawable::UIDrawable(GLuint shader_program, GLuint texture) : FlatDrawable(shader_program){
    window_width = Window::getInstance()->getWidth();
    window_height = Window::getInstance()->getHeight();

    glm::mat3 mesh_projection = glm::mat3( window_width, 0.0f  , 0.0f,
                                      0.0f , window_height, 0.0f,
                                      0.0f , 0.0f  , 1.0f );

    inv_mesh_projection = glm::inverse(mesh_projection);

    attachTexture(texture);

}

void UIDrawable::attachTexture(GLuint texture){
    glBindTexture(GL_TEXTURE_2D, texture);
    int miplevel = 0;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &width_pixels);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &height_pixels);

    glm::vec3 image_size = glm::vec3(width_pixels, height_pixels, 1.0);
    glm::vec3 gl_mesh_size = image_size * inv_mesh_projection;

    width = gl_mesh_size.x;
    height = gl_mesh_size.y;

    FlatDrawable::attachTexture(texture);
}

glm::vec2 UIDrawable::getGLPosition(){
    return glm::vec2( 2.0f*(float(x_pixels)/float(window_width))-1.0f, -2.0f*(float(y_pixels)/float(window_height))+1.0f);
}

void UIDrawable::setGLPosition(glm::vec2 position){
    this->position = glm::vec2(position.x + width, position.y - height);
}

void UIDrawable::draw(){
    FlatDrawable::draw();

    if (outline){
        glUniform1i(glGetUniformLocation(shader_program, "is_outline"), true);
        mesh->drawOutline();
        glUniform1i(glGetUniformLocation(shader_program, "is_outline"), false);
    }
}

void UIDrawable::setCoordinates(glm::vec2 start, glm::vec2 end){
    this->width = (end.x - start.x)/2;
    this->height = (end.y - start.y)/2;
    this->position = glm::vec2(start.x + width, start.y + height);
}
