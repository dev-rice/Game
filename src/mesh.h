#ifndef Mesh_h
#define Mesh_h

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

#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <vector>

#include "shader_loader.h"
#include "mesh_loader.h"

class Mesh {
public:
    Mesh() {;}
    Mesh(const char*, GLfloat);
    
    void draw(glm::mat4*, glm::mat4*, glm::mat4*, glm::vec3);
    void attachShader(GLuint);
    void useTexture(const char*, GLuint);

private:

    GLuint num_faces;
    GLfloat scale;

    // Pointers to all that openGL stuff
    GLuint vao;
    GLuint shader_program;

    GLuint texture_number;
    bool has_texture;
};

#endif