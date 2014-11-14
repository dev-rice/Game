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

class Model {
public:
    Model();
    void draw(glm::mat4*, glm::mat4*, glm::mat4*);
    void useTexture(const char*);

private:
    GLfloat* parseOBJVertices(const char*);
    GLuint*  parseOBJFaces(const char*);


    // Pointers to all that openGL stuff
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint texture;
    GLuint shader_program;

    GLuint model_uniform;
};

#endif