#include "flat_drawable.h"

FlatDrawable::FlatDrawable(FlatMesh* mesh, GLuint shader_program){
    this->mesh = mesh;
    this->shader_program = shader_program;
    this->width = 1.0f;
    this->height = 1.0f;
    this->position = glm::vec2(0.0f, 0.0f);

    this->mesh->attachGeometryToShader(shader_program);
}

FlatDrawable::FlatDrawable(FlatMesh* mesh, GLuint shader_program, GLfloat width, GLfloat height, glm::vec2 position){
    this->mesh = mesh;
    this->shader_program = shader_program;
    this->width = width;
    this->height = height;
    this->position = position;

    this->mesh->attachGeometryToShader(shader_program);
}

void FlatDrawable::draw(){
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    
    glUseProgram(shader_program);
    mesh->bindVAO();
    
    transformation = glm::mat3( width , 0     , position.x,
                                0     , height, position.y,
                                0     , 0     , 1           );
    
    glUniformMatrix3fv(glGetUniformLocation(shader_program, "transformation"), 1, GL_FALSE, glm::value_ptr(transformation));

    updateUniformData();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    mesh->draw();
}

void FlatDrawable::updateUniformData(){
    
}

void FlatDrawable::attachTexture(GLuint texture){
    glUseProgram(shader_program);
    glUniform1i(glGetUniformLocation(shader_program, "base_texture"), 0);

    this->texture = texture;
}
