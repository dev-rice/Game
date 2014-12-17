#ifndef FlatDrawable_h
#define FlatDrawable_h

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
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"

#include "mesh.h"
#include "camera.h"
#include "flat_mesh.h"

class FlatDrawable {
public:
    FlatDrawable(FlatMesh*, GLuint, GLfloat, GLfloat, glm::vec2);

    void draw();
    void attachTexture(GLuint);

    void setPosition(glm::vec2 p) {position = p;}

private:

    FlatMesh* mesh;
    GLuint shader_program;

    GLfloat width;
    GLfloat height;
    glm::vec2 position;

    GLuint texture;

    glm::mat3 transformation;
};

#endif