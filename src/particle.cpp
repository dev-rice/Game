// particle.cpp
// Trevor Westphal

#include "particle.h"
#include "texture_loader.h"
#include "texture_set.h"

Particle::Particle(Mesh* mesh, TextureSet* texture_set, GLuint shader_program){
    drawable = new Drawable(mesh, shader_program);
    drawable->attachTextureSet(*texture_set);
}

void Particle::setInitialValues(glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration, float rotationSpeed, int lifespan, ScalingOption scaleWithAge, FadingOption alphaWithAge){

}

void Particle::draw(Camera* camera, glm::mat4* proj_matrix, Light* light){

    // if(currentTick < decayTicks){
    //     currentTick++;

    //     float scale = 1- (currentTick/float(decayTicks));

    //     drawable->setScale(scale);

    //     float position_scalar = fmax((initialSpeed - currentTick * deceleration), 0.0f);
    //     position +=  initDir * position_scalar;

    //     drawable->moveTo(position);
    //     drawable->setRotation(camera->getRotation());
    //     glm::mat4 view_matrix = camera->getViewMatrix();
    //     drawable->draw(&view_matrix, proj_matrix, light);

    // }



}