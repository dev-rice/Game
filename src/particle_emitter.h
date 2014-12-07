// particle_emitter.h
// Trevor Westphal

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

#include <vector>
#include <random>
#include <deque>


#include "mesh.h"
#include "particle.h"
#include "light.h"
#include "drawable.h"

class ParticleEmitter{
public:
    ParticleEmitter();
    ParticleEmitter(glm::vec3, glm::vec3, int, float, float, float, int);
    void draw(glm::mat4*, glm::mat4*, Light*);
private:
    float getRandomOffset();
    void loadMesh();

    std::deque<Particle> particles;

    glm::vec3 position;
    glm::vec3 initDir;

    int particleCount;
    float initialSpeed;
    float deceleration;
    float decayTicks;
    float randomAmount;
    float halfRandomAmount;

    Mesh plane;
    Drawable drawable;
};