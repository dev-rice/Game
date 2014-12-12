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

    physicsEnabled = false;
}

void Particle::enablePhysics(float elasticity){
    // We will probably need a pointer to the map for the height so 
    // it will bounce realistically
    physicsEnabled = true;
    this->elasticity = elasticity;
}

void Particle::draw(Camera* camera, glm::mat4* proj_matrix){

    if(age < lifespan){
        age++;


        switch(scaleWithAge){
        case ScalingOption::SCALE_DOWN_WITH_AGE:
            drawable->setScale(1.0-(age/float(lifespan)));
            break;
        case ScalingOption::SCALE_UP_WITH_AGE:
            drawable->setScale(age/float(lifespan));
            break;
        case ScalingOption::SCALE_NONE:
            break;
        }

        // TODO add fading with age
        switch(alphaWithAge){
        case FadingOption::FADE_OUT_WITH_AGE:
            // older makes more transparent
            break;
        case FadingOption::FADE_IN_WITH_AGE:
            // older makes more opaque
            break;
        case FadingOption::FADE_NONE:
            break;
        }

        // TODO billboard facing
        // dir = position - camera->getPosition();
        // float x_rot = acos(dir.x/dir.z);
        // float y_rot = acos(dir.y/sqrt(pow(dir.x,2)+pow(dir.z,2)));
        // planeRotation+=rotationSpeed;
        // drawable->setRotation(glm::vec3(x_rot, y_rot, planeRotation));

        velocity += acceleration;

        // Change the zero to be the map height for real pretend physics!
        if(physicsEnabled){
            if(position.y < 0.0f){
                velocity.y = (velocity.y * -1.0f * elasticity);
            }
            if(velocity.y < 0.0005f && velocity.y > -0.0005f && position.y < 0.1f && position.y > -0.1f){
                velocity*=0.5f;
            }
        }

        position += velocity;

        glm::vec3 rotation = camera->getRotation();
        rotation.x = -rotation.x;
        drawable->setRotation(rotation);
        drawable->setPosition(position);
        
        drawable->draw(camera, proj_matrix);

    }
}

bool Particle::isDead(){
    return (!(age<lifespan));
}