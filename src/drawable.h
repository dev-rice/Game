#ifndef Drawable_h
#define Drawable_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/OpenGL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "model.h"

class Drawable {
public:
    Drawable(Model*);
    Drawable(Model*, glm::vec3);

    void draw(glm::mat4*, glm::mat4*);
    
private:
    Model* model;

    glm::vec3 position;
    glm::vec3 rotation;
    
    glm::mat4 model_matrix;
};

#endif