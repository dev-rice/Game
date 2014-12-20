#ifndef FlatMesh_h
#define FlatMesh_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

#include <cstdlib>
#include <vector>
#include <algorithm>

#include "mesh_loader.h"

class FlatMesh {
public:
    FlatMesh();

    void draw();
    void bindVAO();
    void attachGeometryToShader(GLuint);

protected:
    GLuint vao;
    GLuint vbo;

    std::vector<GLuint> bound_shaders;

};

#endif