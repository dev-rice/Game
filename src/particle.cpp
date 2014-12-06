// particle.cpp
// Trevor Westphal

#include "particle.h"

Particle::Particle(glm::vec3 position, glm::vec3 initDir, float initialSpeed, float deceleration, float decay){
    this->position = position;
    this->initDir = initDir;

    this->initialSpeed = initialSpeed;
    this->deceleration = deceleration;
    this->decay = decay;
}

void Particle::setInitialValues(glm::vec3 position, glm::vec3 initDir, float initialSpeed, float deceleration, float decay){
    this->position = position;
    this->initDir = initDir;

    this->initialSpeed = initialSpeed;
    this->deceleration = deceleration;
    this->decay = decay;
}

void Particle::draw(){

}