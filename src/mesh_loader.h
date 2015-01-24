// mesh_loader.h
// Header file for *.obj file importing
// and mesh object creation
// Trevor Westphal
#ifndef MeshLoader_h
#define MeshLoader_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <cmath>
#include <string>

#include "pugixml.hpp" // PUGI xml library

#include "debug.h"


class MeshLoader{
public:
    MeshLoader(const char*);

    std::vector<GLfloat> getVertexArray();
    std::vector<GLuint>  getFaceArray();

    void loadMeshFromDAE(const char*);

private:

    void loadMeshFromOBJ(const char*);

    std::vector<GLuint> final_tris;
    std::vector<GLfloat> final_verts;
};

#endif
