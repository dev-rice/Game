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

#include "mesh_loader.h"
#include "light.h"

class Mesh {
public:
    Mesh() {;}
    Mesh(const char*);
    Mesh(std::vector<GLfloat>, std::vector<GLuint>);

    void draw();
    void bindVAO();
    void attachGeometryToShader(GLuint);
private:

    void loadMeshData(std::vector<GLfloat>, std::vector<GLuint>);

    GLuint num_faces;

    // Pointers to all that openGL stuff
    GLuint vao;

};

#endif