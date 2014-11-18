#ifndef Model_h
#define Model_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/OpenGL.h>

#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <vector>

#include "shader_loader.h"
#include "mesh.h"

class Model {
public:
    Model() {;}
    Model(const char*, GLfloat);
    
    void draw(glm::mat4*, glm::mat4*, glm::mat4*);
    void attachShader(GLuint);
    void useTexture(const char*, GLuint);

private:

    GLuint num_faces;
    GLfloat scale;

    // Pointers to all that openGL stuff
    GLuint vao;
    GLuint shader_program;

    GLuint texture_number;
};

#endif