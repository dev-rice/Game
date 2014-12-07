// particle.cpp
// Trevor Westphal

#include "particle.h"

Particle::Particle(glm::vec3 position, glm::vec3 initDir, float initialSpeed, float deceleration, int decayTicks){
    this->position = position;
    this->initDir = initDir;

    this->initialSpeed = initialSpeed;
    this->deceleration = deceleration;
    this->decayTicks = decayTicks;

    this->currentTicks = 0;
}

void Particle::setInitialValues(glm::vec3 position, glm::vec3 initDir, float initialSpeed){
    this->position = position;
    this->initDir = initDir;

    this->initialSpeed = initialSpeed;

    this->currentTicks = 0;
}

void Particle::draw(){

    if(currentTicks < decayTicks){
        position+=(initDir * initialSpeed * (currentTicks/float(decayTicks)));

        // Now actually draw the damn thing
    }

}