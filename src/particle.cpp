// particle.cpp
// Trevor Westphal

#include "particle.h"

Particle::Particle(glm::vec3 position, glm::vec3 initDir, float initialSpeed, float deceleration, int decayTicks){
    this->position = position;
    this->initDir = initDir;

    this->initialSpeed = initialSpeed;
    this->deceleration = deceleration;
    this->decayTicks = decayTicks;

    this->currentTick = 0;
}

void Particle::setInitialValues(glm::vec3 position, glm::vec3 initDir, float initialSpeed){
    this->position = position;
    this->initDir = initDir;

    this->initialSpeed = initialSpeed;

    this->currentTick = 0;
}

void Particle::draw(){

    if(currentTick < decayTicks){
        currentTick++;

        float something = (currentTick/float(decayTicks));

        position += (initDir * initialSpeed); // Needs deceleration

        // Now actually draw the damn thing
    }

}