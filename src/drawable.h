#ifndef Drawable_h
#define Drawable_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.h"

class Drawable {
public:
    Drawable(Mesh*);
    Drawable(Mesh*, glm::vec3);

    void draw(glm::mat4*, glm::mat4*);
    void moveTo(glm::vec3);
    glm::vec3 getPosition();

private:
    Mesh* mesh;

    glm::vec3 position;
    glm::vec3 rotation;
    
    glm::mat4 model_matrix;
};

#endif