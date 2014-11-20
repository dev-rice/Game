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
    Camera();
    Camera(float, float, float);
    Camera(float, float, float, float, float);

    void moveX(int);
    void moveY(int);
    void moveZ(int);

    void rotateX(int);
    void rotateY(int);
    void rotateZ(int);

    void print();

    glm::mat4 getViewMatrix();

private:
    float x, y, z;
    float x_rot, y_rot, z_rot;

    float move_sensitivity;
    float rotate_sensitivity;
    
    

};

#endif