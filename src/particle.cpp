// particle.cpp
// Trevor Westphal

#include "particle.h"
#include "texture_loader.h"
#include "texture_set.h"

Particle::Particle(Mesh* mesh, TextureSet* texture_set, GLuint shader_program){
    // position = glm::vec3(0.0f, 0.0f, 0.0f);
    // initDir = glm::vec3(0.0f, 0.0f, 0.0f);
    // initialSpeed = 0.0f;
    // deceleration = 0.0f;
    // decayTicks = 100;
    // currentTick = 0;

    drawable = new Drawable(mesh, shader_program);
    // drawable->attachTextureSet(*texture_set);
}

Particle::Particle(glm::vec3 position, glm::vec3 initDir, float initialSpeed, float deceleration, int decayTicks, Mesh* billboard, TextureSet* texture_set, GLuint shader_program){
    this->position = position;
    this->initDir = initDir;

    this->initialSpeed = initialSpeed;
    this->deceleration = deceleration;
    this->decayTicks = decayTicks;

    this->currentTick = 0;

    drawable = new Drawable(billboard, shader_program);
    drawable->attachTextureSet(*texture_set);

}

void Particle::setInitialValues(glm::vec3 position, glm::vec3 initDir, float initialSpeed){
    this->position = position;
    this->initDir = initDir;

    this->initialSpeed = initialSpeed;

    this->currentTick = 0;
}

void Particle::draw(Camera* camera, glm::mat4* proj_matrix, Light* light){

    if(currentTick < decayTicks){
        currentTick++;

        float scale = 1- (currentTick/float(decayTicks));

        drawable->setScale(scale);

        float position_scalar = fmax((initialSpeed - currentTick * deceleration), 0.0f);
        position +=  initDir * position_scalar;

        drawable->moveTo(position);
        drawable->setRotation(camera->getRotation());
        glm::mat4 view_matrix = camera->getViewMatrix();
        drawable->draw(&view_matrix, proj_matrix, light);

    }



}