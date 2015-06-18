#ifndef Mesh_h
#define Mesh_h

#include "includes/gl.hpp"

#include <SOIL.h>

#include "includes/glm.hpp"

#include <cstdlib>
#include <vector>
#include <algorithm>
#include <string>


#include "mesh_loader.h"
#include "debug.h"
#include "vertex.h"
#include "shader.hpp"

class Mesh {
public:
    Mesh() {;}
    Mesh(string filename);
    Mesh(std::vector<GLfloat>, std::vector<GLuint>);
    Mesh(std::vector<Vertex>, std::vector<GLuint>);

    void draw();
    void bindVAO();
    virtual void attachGeometryToShader(Shader& shader);
protected:

    void loadMeshData(std::vector<GLfloat>, std::vector<GLuint>);

    GLuint num_faces;

    GLuint vao;
    GLuint vbo;

    std::vector<Shader*> bound_shaders;

};

#endif
