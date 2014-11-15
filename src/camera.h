#include <glm/glm.hpp>

struct Camera {
    float x, y, z;
    float y_rot;
    float sensitivity;

    Camera() : x(0), y(0), z(0), sensitivity(0.05) {}
    Camera(float x, float y, float z) : x(x), y(y), z(z), sensitivity(0.05) {}
};