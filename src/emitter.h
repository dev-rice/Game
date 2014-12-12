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
#include <math.h>

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

    void draw(Camera*, glm::mat4*);
    void setParticleDensity(int);
    void makeShotgun();
private:
    void prepareParticles(Camera*);

    int maxParticles;
    int density;
    int lifespan;

    bool isShotgun;
    bool hasFired;

    Mesh* billboard;
    Drawable* particle;
    TextureSet* texture_set;

    GLuint shader_program;

    std::deque<Particle*> particles;
};

#endif