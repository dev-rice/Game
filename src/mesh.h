// mesh.h
// Header file for *.obj file importing
// and mesh object creation
// Trevor Westphal

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

#include <vector>


class Mesh{
public:
	Mesh(const char*);
	
    GLfloat* getVertexArray();
	GLuint* getFaceArray();
    
    int getVerticesSize();
    int getFacesSize();
private:
    
    void loadMeshFromFile(const char*);

	std::vector<GLuint> final_tris;
	std::vector<GLfloat> final_verts;
};
