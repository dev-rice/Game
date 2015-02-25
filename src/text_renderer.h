#ifndef TextRenderer_h
#define TextRenderer_h

#include <GL/glew.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <unordered_map>

#include "character_drawable.h"
#include "character_mesh.h"
#include "shader_loader.h"
#include "texture_loader.h"
#include "window.h"

class TextRenderer {
public:
    TextRenderer(std::string, GLint);

    void print(int, int, const char*, ...);
private:
    void drawString(std::string);
    void drawStringInitial(int x, int y, std::string to_draw);

    CharacterMesh* character_mesh;
    CharacterDrawable* character_box;

    std::unordered_map<std::string, glm::vec3> colors;

    GLfloat point;
};

#endif
