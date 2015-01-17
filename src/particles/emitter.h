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
#include <ctime>
#include <random>
#include <algorithm>
#include <math.h>

#include "mesh.h"
#include "camera.h"
#include "texture_loader.h"


#include "particle.h"

class Emitter {
public:
    ~Emitter();
    Emitter(GLuint, glm::vec3);

    void draw(Camera*);
    void setParticleDensity(int);
    void makeShotgun();
protected:
    virtual void prepareParticles(Camera*);

    float old_time;

    int maxParticles;
    int density;
    int lifespan;

    glm::vec3 position;

    bool isShotgun;
    bool hasFired;

    Mesh* billboard;
    GLuint particle_texture;

    GLuint shader_program;

    std::deque<Particle*> particles;
};

#endif