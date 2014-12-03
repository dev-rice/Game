#include "camera.h"

Camera::Camera(){
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);

    local_x = glm::vec3(1.0f, 0.0f, 0.0f);
    local_y = glm::vec3(0.0f, 1.0f, 0.0f);
    local_z = glm::vec3(0.0f, 0.0f, 1.0f);

    this->move_sensitivity = MOVE_SENSITIVITY;
    this->rotate_sensitivity = ROTATE_SENSITIVITY;
}

Camera::Camera(glm::vec3 position){
    this->position = position;

    local_x = glm::vec3(1.0f, 0.0f, 0.0f);
    local_y = glm::vec3(0.0f, 1.0f, 0.0f);
    local_z = glm::vec3(0.0f, 0.0f, 1.0f);

    this->move_sensitivity = MOVE_SENSITIVITY;
    this->rotate_sensitivity = ROTATE_SENSITIVITY;
}

Camera::Camera(glm::vec3 position, float move_sensitivity, float rotate_sensitivity){
    this->position = position;

    local_x = glm::vec3(1.0f, 0.0f, 0.0f);
    local_y = glm::vec3(0.0f, 1.0f, 0.0f);
    local_z = glm::vec3(0.0f, 0.0f, 1.0f);

    this->move_sensitivity = move_sensitivity;
    this->rotate_sensitivity = rotate_sensitivity;
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

    // X rotation matrix 
    glm::mat3 x_rotation_matrix = glm::mat3( 1.0f, 0.0f      , 0.0f,
                                             0.0f, cos(rotation.x), -sin(rotation.x),
                                             0.0f, sin(rotation.x), cos(rotation.x));

    // Y rotation matrix
    glm::mat3 y_rotation_matrix = glm::mat3( cos(rotation.y) , 0.0f, sin(rotation.y),
                                             0.0f            , 1.0f, 0.0f,
                                             -sin(rotation.y), 0.0f, cos(rotation.y));
    y_rotation_matrix = glm::inverse(y_rotation_matrix);

    // Z rotation matrix
    glm::mat3 z_rotation_matrix = glm::mat3( cos(rotation.z), -sin(rotation.z), 0.0f,
                                             sin(rotation.z),  cos(rotation.z), 0.0f,
                                             0.0f           , 0.0f            , 1.0f);

    glm::mat3 rotation_matrix = z_rotation_matrix * y_rotation_matrix * x_rotation_matrix;

    // Transform the center vector
    center = position - (rotation_matrix * glm::vec3(0.0f, 0.0f, 1.0f));
    
    // Transform the camera axes
    local_x = rotation_matrix * glm::vec3(1.0f, 0.0f, 0.0f);
    local_y = rotation_matrix * glm::vec3(0.0f, 1.0f, 0.0f);
    local_z = rotation_matrix * glm::vec3(0.0f, 0.0f, 1.0f);

    // Debug
    print();

    glm::mat4 view_matrix = glm::lookAt(eye, center, up);

    return view_matrix;
}

glm::vec3 Camera::getPosition(){
    return position;
}

void Camera::print(){
    printf("Camera\n");
    printf("    position = <%f, %f, %f>\n", position.x, position.y, position.z);
    printf("    rotation = <%f, %f, %f>\n\n", 180 * rotation.x / M_PI, 180 * rotation.y / M_PI, 180 * rotation.z / M_PI);
    printf("    local_x  = <%f, %f, %f>\n", local_x.x, local_x.y, local_x.z);
    printf("    local_y  = <%f, %f, %f>\n", local_y.x, local_y.y, local_y.z);
    printf("    local_z  = <%f, %f, %f>\n", local_z.x, local_z.y, local_z.z);
}