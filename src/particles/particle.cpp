// particle.cpp
// Trevor Westphal

#include "particle.h"
#include "texture_loader.h"


Particle::Particle(Mesh* mesh, GLuint shader_program) : Drawable(mesh, shader_program){

}

Particle::Particle(Mesh* mesh, GLuint shader_program, glm::vec3 position, GLfloat scale): Drawable(mesh, shader_program, position, scale) {

}

Particle::Particle(Mesh* mesh, GLuint shader_program, glm::vec3 position, glm::vec3 rotation, GLfloat scale): Drawable(mesh, shader_program, position, rotation, scale) {

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
    this->plane_rotation = 0.0f;

    physicsEnabled = false;
}

void Particle::enablePhysics(float elasticity){
    // We will probably need a pointer to the map for the height so
    // it will bounce realistically
    physicsEnabled = true;
    this->elasticity = elasticity;
}

void Particle::update(){
    if(age < lifespan){
        age++;

        switch(scaleWithAge){
        case ScalingOption::SCALE_DOWN_WITH_AGE:
            scale = 1.0-(age/float(lifespan));
            break;
        case ScalingOption::SCALE_UP_WITH_AGE:
            scale = age/float(lifespan);
            break;
        case ScalingOption::SCALE_SMOKE:
            scale = 2.0f + age/float(lifespan);
            break;
        case ScalingOption::SCALE_NONE:
            scale = 1.0f;
            break;
        }

        // TODO add fading with age
        switch(alphaWithAge){
        case FadingOption::FADE_OUT_WITH_AGE:
            // older makes more transparent
            opacity = 1.0f-(0.5f*age/float(lifespan));
            break;
        case FadingOption::FADE_IN_WITH_AGE:
            // older makes more opaque
            opacity = 0.5f*age/float(lifespan);
            break;
        case FadingOption::FADE_SMOKE:
            opacity = 0.6f-0.0000003f*(age-700)*(age-700);
            break;
        case FadingOption::FADE_NONE:
            opacity = 1.0f;
            break;
        }

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

        plane_rotation += rotationSpeed;
    }
}

void Particle::draw(){
    if(age < lifespan){
        Drawable::draw();
    }
}

void Particle::updateUniformData(){
    // Set the scale, this is not really going to be a thing, probably
    // ^ It's definitely a thing
    glUniform1f(glGetUniformLocation(shader_program, "scale"), scale);

    // Set the opacity in the shader
    glUniform1f(glGetUniformLocation(shader_program, "opacity"), (float)opacity);

    // Set the planar rotation
    glUniform1f(glGetUniformLocation(shader_program, "plane_rotation"), (float)plane_rotation);

    // Tell the shader the current time
    // glUniform1f(glGetUniformLocation(shader_program, "time"), (float)glfwGetTime());

}

bool Particle::isDead(){
    return (!(age<lifespan));
}