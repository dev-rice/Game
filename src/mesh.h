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

#include <cstdlib>
#include <vector>
#include <algorithm>
#include <string>

#include "shader_loader.h"
#include "mesh_loader.h"
#include "debug.h"
#include "vertex.h"

class Mesh {
public:
    Mesh() {;}
    Mesh(const char*);
    Mesh(std::vector<GLfloat>, std::vector<GLuint>);
    Mesh(std::vector<Vertex>, std::vector<GLuint>);

    void draw();
    void bindVAO();
    virtual void attachGeometryToShader(GLuint);
protected:

    void loadMeshData(std::vector<GLfloat>, std::vector<GLuint>);

    GLuint num_faces;

    GLuint vao;
    GLuint vbo;

    std::vector<GLuint> bound_shaders;

};

#endif
