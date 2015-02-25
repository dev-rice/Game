#ifndef FlatMesh_h
#define FlatMesh_h

#include <GL/glew.h>

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
    static FlatMesh* getInstance();

    void draw();
    void drawOutline();
    void bindVAO();
    void attachGeometryToShader(GLuint);

protected:
    GLuint vao;
    GLuint vbo;

    std::vector<GLuint> bound_shaders;

    FlatMesh();
    static FlatMesh* instance;

};

#endif
