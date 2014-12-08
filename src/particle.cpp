// particle.cpp
// Trevor Westphal

#include "particle.h"
#include "texture_loader.h"
#include "texture_set.h"

Particle::Particle(glm::vec3 position, glm::vec3 initDir, float initialSpeed, float deceleration, int decayTicks, Mesh* billboard, TextureSet* texture_set){
    this->position = position;
    this->initDir = initDir;

    this->initialSpeed = initialSpeed;
    this->deceleration = deceleration;
    this->decayTicks = decayTicks;

    this->currentTick = 0;

    drawable = new Drawable(billboard);
    drawable->attachTextureSet(*texture_set);

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

        float something = (currentTick/float(decayTicks));

        float position_scalar = fmax((initialSpeed - currentTick * deceleration), 0.0f);
        position +=  initDir * position_scalar;

        drawable->moveTo(position);
        drawable->draw(view_matrix, proj_matrix, light);

    }



}