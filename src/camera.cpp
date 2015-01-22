#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 rotation, float move_sensitivity, float rotate_sensitivity){
    this->position = position;
    this->rotation = rotation;

    local_x = glm::vec3(1.0f, 0.0f, 0.0f);
    local_y = glm::vec3(0.0f, 1.0f, 0.0f);
    local_z = glm::vec3(0.0f, 0.0f, 1.0f);

    this->move_sensitivity = move_sensitivity;
    this->rotate_sensitivity = rotate_sensitivity;
}

void Camera::setPosition(glm::vec3 position){
    this->position = position;
}

void Camera::setRotation(glm::vec3 rotation){
    this->rotation = rotation;
}

void Camera::moveX(int direction){
    position += move_sensitivity * direction * local_x;
}

void Camera::moveY(int direction){
    position += move_sensitivity * direction * local_y;
}

void Camera::moveZ(int direction){
    position += move_sensitivity * direction * local_z;
}

void Camera::moveGlobalX(int direction){
    position.x += move_sensitivity * direction;
}

void Camera::moveGlobalY(int direction){
    position.y += move_sensitivity * direction;
}

void Camera::moveGlobalZ(int direction){
    position.z += move_sensitivity * direction;
}

void Camera::rotateX(int direction){
    rotation.x += rotate_sensitivity * direction;
}

void Camera::rotateY(int direction){
    rotation.y += rotate_sensitivity * direction;
}

void Camera::rotateZ(int direction){
    rotation.z += rotate_sensitivity * direction;
}

glm::mat4 Camera::getViewMatrix(){
    // Original vectors
    // eye      <x, y, z>
    // center   <x, y, z> - <0, 0, 1>
    // up       <0, 1, 0>
    glm::vec3 eye    = position;
    glm::vec3 center = position - glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 up     = local_y;

    float cx = cos(rotation.x);
    float sx = sin(rotation.x);

    float cy = cos(rotation.y);
    float sy = sin(rotation.y);

    glm::mat3 rotation_x = glm::mat3(  1 ,  0 ,  0 ,
                                       0 ,  cx, -sx,
                                       0 ,  sx,  cx  );

    glm::mat3 rotation_y = glm::mat3(  cy,  0 , -sy,
                                       0 ,  1 ,  0 ,
                                       sy,  0 ,  cy  );

    glm::mat3 rotation_matrix = rotation_y * rotation_x;

    // Transform the center vector
    center = position - (rotation_matrix * glm::vec3(0.0f, 0.0f, 1.0f));

    // Transform the camera axes
    local_x = rotation_matrix * glm::vec3(1.0f, 0.0f, 0.0f);
    local_y = rotation_matrix * glm::vec3(0.0f, 1.0f, 0.0f);
    local_z = rotation_matrix * glm::vec3(0.0f, 0.0f, 1.0f);

    glm::mat4 view_matrix = glm::lookAt(eye, center, up);

    return view_matrix;
}

glm::vec3 Camera::getPosition(){
    return position;
}

glm::vec3 Camera::getRotation(){
    return rotation;
}
