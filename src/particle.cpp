// particle.cpp
// Trevor Westphal

#include "particle.h"
#include "texture_loader.h"
#include "texture_set.h"

Particle::Particle(glm::vec3 position, glm::vec3 initDir, float initialSpeed, float deceleration, int decayTicks, Mesh* plane){
    this->position = position;
    this->initDir = initDir;

    this->initialSpeed = initialSpeed;
    this->deceleration = deceleration;
    this->decayTicks = decayTicks;

    this->currentTick = 0;

    this->particleDrawable = Drawable(plane, glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
    TextureSet set = TextureSet(0, 0, 0, 0);
    this->particleDrawable.attachTextureSet(set);
}

void Particle::setInitialValues(glm::vec3 position, glm::vec3 initDir, float initialSpeed){
    this->position = position;
    this->initDir = initDir;

    this->initialSpeed = initialSpeed;

    this->currentTick = 0;
}

void Particle::draw(glm::mat4* view_matrix, glm::mat4* proj_matrix, Light* light){

    if(currentTick < decayTicks){
        currentTick++;

        //float something = (currentTick/float(decayTicks));

        //position += (initDir * initialSpeed); // Needs deceleration

        // Now actually draw the damn thing
        // particleDrawable.moveTo(position);
        // particleDrawable.draw(view_matrix, proj_matrix, light);
    }
    particleDrawable.draw(view_matrix, proj_matrix, light);


}