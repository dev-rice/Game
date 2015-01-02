#ifndef Camera_h
#define Camera_h

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>

const float MOVE_SENSITIVITY = 0.1;
const float ROTATE_SENSITIVITY = 0.025;

class Camera {
public:
    Camera() : Camera(glm::vec3(), MOVE_SENSITIVITY, ROTATE_SENSITIVITY) {;}
    Camera(glm::vec3 p) : Camera(p, MOVE_SENSITIVITY, ROTATE_SENSITIVITY) {;}
    Camera(glm::vec3, float, float);

    void moveX(int);
    void moveY(int);
    void moveZ(int);

    void rotateX(int);
    void rotateY(int);
    void rotateZ(int);

    void moveGlobalZ(int);

    void setRotation(glm::vec3);

    void print();

    glm::vec3 getPosition();
    glm::vec3 getRotation();
    glm::mat4 getViewMatrix();

private:
    glm::vec3 position;
    glm::vec3 rotation;

    glm::vec3 local_x;
    glm::vec3 local_y;
    glm::vec3 local_z;

    float move_sensitivity;
    float rotate_sensitivity;

};


#endif
