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

#include <deque>
#include <random>

#include "drawable.h"
#include "mesh.h"
#include "camera.h"
#include "shader_loader.h"
#include "light.h"
#include "texture_loader.h"
#include "texture_set.h"
#include "particle.h"

class Emitter {
public:
    Emitter();
    ~Emitter();
    Emitter(GLuint);

    void draw(glm::mat4*, glm::mat4*, Light*);
private:
    int maxParticles;

    Mesh* billboard;
    TextureSet* texture_set;

    std::deque<Particle*> particles;

};

#endif