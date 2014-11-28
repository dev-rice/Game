#ifndef Camera_h
#define Camera_h

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>

class Camera {
public:
    Camera();
    Camera(float, float, float);
    Camera(float, float, float, float, float, float);

    void moveX(float);
    void moveY(float);
    void moveZ(float);

    void rotateLocalX(float);
    void rotateLocalY(float);
    void rotateLocalZ(float);

    void rotateGlobalX(float);
    void rotateGlobalY(float);
    void rotateGlobalZ(float);

    void print();

    glm::vec3 getPosition();
    glm::mat4 getViewMatrix();

private:
    float x, y, z;
    float x_rot, y_rot, z_rot;

};

#endif