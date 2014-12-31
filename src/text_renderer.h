#ifndef TextRenderer_h
#define TextRenderer_h

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

#include "character_drawable.h"
#include "character_mesh.h"
#include "shader_loader.h"
#include "texture_loader.h"

class TextRenderer {
public:
    TextRenderer(GLFWwindow*, std::string, GLfloat);

    void print(glm::vec2, const char*, ...);
private:
    void drawString(glm::vec2, std::string);
    
    CharacterMesh* character_mesh;
    CharacterDrawable* character_box;

    GLfloat scale;
};

#endif
