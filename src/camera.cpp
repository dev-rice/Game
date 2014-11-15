#include "camera.h"



Camera::Camera(){
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;

    this->move_sensitivity = MOVE_SENSITIVITY;
    this->rotate_sensitivity = ROTATE_SENSITIVITY;
}

Camera::Camera(float x, float y, float z){
    this->x = x;
    this->y = y;
    this->z = z;

    this->move_sensitivity = MOVE_SENSITIVITY;
    this->rotate_sensitivity = ROTATE_SENSITIVITY;
}

Camera::Camera(float x, float y, float z, float move_sensitivity, float rotate_sensitivity){
    this->x = x;
    this->y = y;
    this->z = z;

    this->move_sensitivity = move_sensitivity;
    this->rotate_sensitivity = rotate_sensitivity;
}

void Camera::moveX(int direction){
    x += move_sensitivity * direction * cos(y_rot);
    z += move_sensitivity * direction * sin(y_rot);
}

void Camera::moveY(int direction){
    y += move_sensitivity * direction;
}

void Camera::moveZ(int direction){
    x -= move_sensitivity * direction * sin(y_rot);
    z += move_sensitivity * direction * cos(y_rot);
}

void Camera::rotateX(int direction){
    x_rot += rotate_sensitivity * direction;
}

void Camera::rotateY(int direction){
    y_rot += rotate_sensitivity * direction;
}

void Camera::rotateZ(int direction){
    z_rot += rotate_sensitivity * direction;
}

glm::mat4 Camera::getViewMatrix(){
    glm::mat4 view_matrix = glm::lookAt(
        glm::vec3(x, y, z),
        glm::vec3(x + sin(y_rot), y, z - cos(y_rot)),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    return view_matrix;
}