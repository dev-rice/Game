#include "drawable.h"

Drawable::Drawable(Mesh* mesh, GLuint shader_program){
    load(mesh, shader_program, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
}

Drawable::Drawable(Mesh* mesh, GLuint shader_program, glm::vec3 position, GLfloat scale) {
    load(mesh, shader_program, position, glm::vec3(0.0f, 0.0f, 0.0f), scale);
}

Drawable::Drawable(Mesh* mesh, GLuint shader_program, glm::vec3 position, glm::vec3 rotation, GLfloat scale) {
    load(mesh, shader_program, position, rotation, scale);

}

void Drawable::load(Mesh* mesh, GLuint shader_program, glm::vec3 position, glm::vec3 rotation, GLfloat scale) {
    this->position = position;
    this->rotation = rotation;
    this->mesh = mesh;
    this->scale = scale;
    
    this->shader_program = shader_program;
    this->mesh->attachGeometryToShader(shader_program);

    glUniform1i(glGetUniformLocation(shader_program, "diffuse_texture"), 0);
    glUniform1i(glGetUniformLocation(shader_program, "specular_texture"), 1);
    glUniform1i(glGetUniformLocation(shader_program, "normal_map"), 2);
    glUniform1i(glGetUniformLocation(shader_program, "emissive_texture"), 3);
}

void Drawable::moveTo(glm::vec3 new_position){
    position = new_position;
}

glm::vec3 Drawable::getPosition(){
    return position;
}

void Drawable::setRotation(glm::vec3 rotation){
    this->rotation = rotation;
}

void Drawable::setScale(GLfloat scale){
    this->scale = scale;
}

void Drawable::draw(glm::mat4* view_matrix, glm::mat4* proj_matrix, Light* light){
    mesh->bindVAO();

    // Create the model matrix based on position
    model_matrix = glm::translate(glm::mat4(), position);

    glm::vec3 x_axis = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 y_axis = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 z_axis = glm::vec3(0.0f, 0.0f, 1.0f);

    model_matrix = glm::rotate(model_matrix, rotation.x, x_axis);
    model_matrix = glm::rotate(model_matrix, rotation.y, y_axis);
    model_matrix = glm::rotate(model_matrix, rotation.z, z_axis);

    // Set the scale, this is not really going to be a thing, probably
    glUniform1f(glGetUniformLocation(shader_program, "scale"), scale);

    // Update the time uniform
    glUniform1f(glGetUniformLocation(shader_program, "time"), (float)glfwGetTime());

    glUniform3fv(glGetUniformLocation(shader_program, "light.position"), 1, light->getPosition());
    glUniform3fv(glGetUniformLocation(shader_program, "light.color"), 1, light->getColor());
    glUniform1fv(glGetUniformLocation(shader_program, "light.intensity"), 1, light->getIntensity());

    // Update the model, view, and projection matrices
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, glm::value_ptr(model_matrix));
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, glm::value_ptr(*view_matrix));    
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "proj"), 1, GL_FALSE, glm::value_ptr(*proj_matrix));

    bindTextures();

    mesh->draw();
}

void Drawable::bindTextures(){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_set.diffuse);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_set.specular);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture_set.normal);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, texture_set.emissive);
}
void Drawable::attachTextureSet(TextureSet texture_set){
    this->texture_set = texture_set;
}