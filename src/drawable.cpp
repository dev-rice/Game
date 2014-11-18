#include "drawable.h"

Drawable::Drawable(Model* model){
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->model = model;
}

Drawable::Drawable(Model* model, glm::vec3 position) {
    this->position = position;
    this->model = model;
}

void Drawable::moveTo(glm::vec3 new_position){
    position = new_position;
}

glm::vec3 Drawable::getPosition(){
    return position;
}

void Drawable::draw(glm::mat4* view_matrix, glm::mat4* proj_matrix){
    
    // Create the model matrix based on position
    model_matrix = glm::translate(glm::mat4(), position);

    // x rotation
    model_matrix = glm::rotate(model_matrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    // y rotation
    model_matrix = glm::rotate(model_matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    // z rotation
    model_matrix = glm::rotate(model_matrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    model->draw(view_matrix, proj_matrix, &model_matrix);
}