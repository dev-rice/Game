#include "camera.h"

Camera::Camera(){
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
}

Camera::Camera(float x, float y, float z){
    this->x = x;
    this->y = y;
    this->z = z;
}

Camera::Camera(float x, float y, float z, float x_rot, float y_rot, float z_rot){
    this->x = x;
    this->y = y;
    this->z = z;

    this->x_rot = x_rot;
    this->y_rot = y_rot;
    this->z_rot = z_rot;
}

void Camera::moveX(float moveAmount){
    x += moveAmount;
}

void Camera::moveY(float moveAmount){
    y += moveAmount;
}

void Camera::moveZ(float moveAmount){
    z += moveAmount;
}

void Camera::rotateLocalX(float rotateAmount){
    // x_rot += rotate_sensitivity * direction;
}

void Camera::rotateLocalY(float rotateAmount){
    // y_rot += rotate_sensitivity * direction;
}

void Camera::rotateLocalZ(float rotateAmount){
    // z_rot += rotate_sensitivity * direction;
}

void Camera::rotateGlobalX(float rotateAmount){

}

void Camera::rotateGlobalY(float rotateAmount){

}

void Camera::rotateGlobalZ(float rotateAmount){

}

glm::mat4 Camera::getViewMatrix(){
    glm::vec4 unit = glm::vec4(x, y, z - 1.0f, 1.0f);
    
    glm::mat4 view_matrix = glm::lookAt(
        glm::vec3(x, y, z),
        glm::vec3(x - sin(y_rot), y, z - cos(y_rot)),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    return view_matrix;
}

glm::vec3 Camera::getPosition(){
    return glm::vec3(x, y, z);
}

void Camera::print(){
    printf("Camera\n");
    printf("    position = [%f, %f, %f]\n", x, y, z);
    printf("    rotation = [%f, %f, %f]\n", 180 * x_rot / M_PI, 180 * y_rot / M_PI, 180 * z_rot / M_PI);
}