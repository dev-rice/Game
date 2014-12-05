#ifndef Light_h
#define Light_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

#include <SOIL.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light {
public:
    Light();
    Light(glm::vec3, glm::vec3, GLfloat);

    GLfloat* getPosition();
    GLfloat* getColor();
    GLfloat* getIntensity();
private:
    glm::vec3 position;
    glm::vec3 color;
    GLfloat intensity;

};

#endif