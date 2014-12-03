#include "drawable.h"

Drawable::Drawable(Mesh* mesh){
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->mesh = mesh;
}

Drawable::Drawable(Mesh* mesh, glm::vec3 position) {
    this->position = position;
    this->mesh = mesh;
}

Drawable::Drawable(Mesh* mesh, glm::vec3 position, glm::vec3 rotation) {
    this->position = position;
    this->rotation = rotation;
    this->mesh = mesh;
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

void Drawable::draw(glm::mat4* view_matrix, glm::mat4* proj_matrix){
    
    // Create the model matrix based on position
    model_matrix = glm::translate(glm::mat4(), position);

    glm::vec3 x_axis = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 y_axis = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 z_axis = glm::vec3(0.0f, 0.0f, 1.0f);

    model_matrix = glm::rotate(model_matrix, rotation.x, x_axis);
    model_matrix = glm::rotate(model_matrix, rotation.y, y_axis);
    model_matrix = glm::rotate(model_matrix, rotation.z, z_axis);

    mesh->draw(view_matrix, proj_matrix, &model_matrix);
}