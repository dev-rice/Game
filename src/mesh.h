// mesh.h
// Header file for *.obj file importing
// and mesh object creation
// Trevor Westphal

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/OpenGL.h>

#include <vector>


class Mesh{
public:
	Mesh();
	void loadMeshFromFile(const char fileName[]);
	
    GLfloat* getVertexArray();
	GLuint* getFaceArray();
    
    int getVerticesSize();
    int getFacesSize();
private:
	std::vector<GLuint> final_tris;
	std::vector<GLfloat> final_verts;
};
