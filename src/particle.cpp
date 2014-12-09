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
    this->position = position;
    this->velocity = velocity;
    this->acceleration = acceleration;

    this->rotationSpeed = rotationSpeed;

    this->scaleWithAge = scaleWithAge;
    this->alphaWithAge = alphaWithAge;

    this->lifespan = lifespan;

    this->age = 0;
    this->planeRotation = 0.0f;
}

void Particle::draw(Camera* camera, glm::mat4* proj_matrix, Light* light){

    if(age < lifespan){
        age++;


        drawable->setScale(1.0-(age/float(lifespan)));

        // dir = position - camera->getPosition();
        // float x_rot = acos(dir.x/dir.z);
        // float y_rot = acos(dir.y/sqrt(pow(dir.x,2)+pow(dir.z,2)));
        // planeRotation+=rotationSpeed;
        // drawable->setRotation(glm::vec3(x_rot, y_rot, planeRotation));

        velocity += acceleration;
        // Change the zero to be the map height for real pretend physics!
        if(position.y < 0.0f){
            velocity.y = (velocity.y * -0.8f);
        }
        if(velocity.y < 0.0005f && velocity.y > -0.0005f && position.y < 0.1f && position.y > -0.1f){
            velocity*=0.5f;
        }
        position += velocity;

        drawable->moveTo(position);
        
        glm::mat4 view_matrix = camera->getViewMatrix();
        drawable->draw(&view_matrix, proj_matrix, light);

    }
}

bool Particle::isDead(){
    return (!(age<lifespan));
}