// particle_emitter.cpp
// Trevor Westphal

#include <random>

#include "particle_emitter.h"

ParticleEmitter::ParticleEmitter(){
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->initDir = glm::vec3(0.0f, 0.0f, 0.0f);

    this->initialSpeed = 1.0f;
    this->deceleration = 1.0f;
    this->decay = 1.0f;
    this->randomAmount = 0.0f;
}

ParticleEmitter::ParticleEmitter(glm::vec3 position, glm::vec3 initDir, int particleCount, float initialSpeed, float deceleration, float decay, float randomAmount){
    this->position = position;
    this->initDir = initDir;

    this->particleCount = particleCount;

    this->initialSpeed = initialSpeed;
    this->deceleration = deceleration;
    this->decay = decay;
    this->randomAmount = randomAmount;
    this->halfRandomAmount = randomAmount/2;
}

void ParticleEmitter::draw(){

    glm::vec3 newPosition = position + glm::vec3(getRandomOffset(), getRandomOffset(), getRandomOffset());
    glm::vec3 newInitDir  = initDir  + glm::vec3(getRandomOffset(), getRandomOffset(), getRandomOffset());

    float newInitSpeed = initialSpeed + getRandomOffset();

    // Can grow up to a size of particleCount, but not bigger,
    // recylcing particle objects
    if(particles.size() < particleCount){ 
        Particle p(newPosition, newInitDir, newInitSpeed, deceleration, decay);
        // Randomize this shit
        particles.push_back(p);
    } else {
        Particle front = particles[0];
        particles.pop_front();
        front.setInitialValues(newPosition, newInitDir, newInitSpeed);
        particles.push_back(front);
    }

    for(int i(0); i < particles.size(); ++i){
        particles[i].draw();
    }
}

float ParticleEmitter::getRandomOffset(){
    // Returns a value between -randomAmount/2 and +randomAmount/2.
    // This is to be added to the value you want randomized

    int scalar = (int)randomAmount*8;
    return (((float)(rand()%scalar)/8.0f) - halfRandomAmount);
}