// particle_emitter.cpp
// Trevor Westphal

#include "particle_emitter.h"

ParticleEmitter::ParticleEmitter(){
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->accelDir = glm::vec3(0.0f, 0.0f, 0.0f);

    this->initialSpeed = 1.0f;
    this->deceleration = 1.0f;
    this->decay = 1.0f;
    this->randomAmount = 0.0f;
}

ParticleEmitter::ParticleEmitter(glm::vec3 position, glm::vec3 accelDir, int particleCount, float initialSpeed, float deceleration, float decay, float randomAmount){
    this->position = position;
    this->accelDir = accelDir;

    this->particleCount = particleCount;

    this->initialSpeed = initialSpeed;
    this->deceleration = deceleration;
    this->decay = decay;
    this->randomAmount = randomAmount;
}

void ParticleEmitter::draw(){

}