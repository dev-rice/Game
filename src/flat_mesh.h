#ifndef FlatMesh_h
#define FlatMesh_h

#include "includes/gl.hpp"

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
