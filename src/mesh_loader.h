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
#include <algorithm>
#include <map>

#include "pugixml.hpp" // PUGI xml library

#include "debug.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 binormal;
    glm::vec2 texcoord;

    bool operator==(const Vertex& rhs){
        return (position == rhs.position) && (normal == rhs.normal) &&
            (texcoord == rhs.texcoord);
    }
};

class MeshLoader{
public:
    MeshLoader(const char*);

    std::vector<GLfloat> getVertexArray() {return final_vertices;}
    std::vector<GLuint>  getFaceArray() {return final_faces;}

    void loadMeshFromDAE(const char*);

private:

    void loadMeshFromOBJ(const char*);

    void writeFinalArrays(std::vector<Vertex>&, std::vector<GLuint>&);
    void calculateTangentsAndBinormals(std::vector<Vertex>&, std::vector<GLuint>&);
    bool getVerticesAndElements(pugi::xml_node, std::vector<Vertex>&, std::vector<GLuint>&);

    std::vector<GLfloat> final_vertices;
    std::vector<GLuint> final_faces;

    std::vector<Vertex> unique_vertices;
    std::map<int, int> vertex_to_unique;

    const char* filename;
};

#endif
