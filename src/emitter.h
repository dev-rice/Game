#ifndef Emitter_h
#define Emitter_h
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

#include "drawable.h"
#include "mesh.h"
#include "camera.h"
#include "shader_loader.h"
#include "light.h"
#include "texture_loader.h"
#include "texture_set.h"

class Emitter {
public:
    Emitter();
    Emitter(GLuint);
    Emitter(Mesh, Drawable);

    void draw(glm::mat4*, glm::mat4*, Light*);
private:
    Drawable particle;
    Mesh billboard;

};

#endif