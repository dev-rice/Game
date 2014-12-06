// particle_emitter.h
// Trevor Westphal

#include <glm/glm.hpp>

#include <deque>

#include "particle.h"

class ParticleEmitter{
public:
    ParticleEmitter();
    ParticleEmitter(glm::vec3, glm::vec3, int, float, float, float, float);
    void draw();
private:
    std::deque<Particle> particles;

    glm::vec3 position;
    glm::vec3 initDir;

    int particleCount;
    float initialSpeed;
    float deceleration;
    float decay;
    float randomAmount;
};