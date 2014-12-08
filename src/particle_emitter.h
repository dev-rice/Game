// particle_emitter.h
// Trevor Westphal

#ifndef ParticleEmitter_h
#define ParticleEmitter_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

// #define GLM_FORCE_RADIANS
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <random>
#include <deque>


#include "particle.h"
#include "drawable.h"
#include "mesh.h"
#include "camera.h"
#include "shader_loader.h"
#include "light.h"
#include "texture_loader.h"
#include "texture_set.h"

class ParticleEmitter{
public:
    ParticleEmitter() {;}
    ParticleEmitter(GLuint);
    ParticleEmitter(Drawable);
    ParticleEmitter(glm::vec3, glm::vec3, int, float, float, float, int, GLuint);
    void draw(glm::mat4*, glm::mat4*, Light*);

    Drawable plane_draw;

private:
    float getRandomOffset();

    std::deque<Particle> particles;

    glm::vec3 position;
    glm::vec3 initDir;
   
    int particleCount;
    float initialSpeed;
    float deceleration;
    float decayTicks;
    float randomAmount;
    float halfRandomAmount;

    Mesh* plane_pointer;
    Mesh plane;

};

#endif