// particle_emitter.h
// Trevor Westphal

#warning Remove me once particles have been included at a higher level
#include <glm/glm.hpp>

#include <vector>

#include "particle.h"

class ParticleEmitter{
public:
    ParticleEmitter();
    ParticleEmitter(glm::vec3, glm::vec3, int, float, float, float, float);
    void draw();
private:
    void populateParticles();

    std::vector<Particle> particles;

    glm::vec3 position;
    glm::vec3 accelDir;

    int particleCount;
    float initialSpeed;
    float deceleration;
    float decay;
    float randomAmount;
};