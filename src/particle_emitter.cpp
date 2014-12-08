// particle_emitter.cpp
// Trevor Westphal

#include "particle_emitter.h"

ParticleEmitter::ParticleEmitter(GLuint shader_program){
    this->position = glm::vec3(0.0f, 1.0f, 1.0f);
    this->initDir = glm::vec3(0.0f, 1.0f, 0.0f);

    this->particleCount = 1;
    this->initialSpeed = 1.0f;
    this->deceleration = 1.0f;
    this->decayTicks = 60;
    this->randomAmount = 0.0f;

    GLfloat planeVerts[] = {-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                             0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                             0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,};

    GLuint  planeFaces[] = {0, 1, 2, 1, 2, 3};

    std::vector<GLfloat> planeVertsVector(planeVerts, planeVerts + sizeof(planeVerts) / sizeof(GLfloat));
    std::vector<GLuint> planeFacesVector(planeFaces, planeFaces + sizeof(planeFaces) / sizeof(GLuint));


    plane = Mesh(planeVertsVector, planeFacesVector);
    plane.attachShader(shader_program);

    plane_pointer = &plane;

    position = glm::vec3(-2.0f, 2.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    GLfloat the_scale = 2.0f;

    TextureSet texture_set(0, 0, 0, 0);

    printf("plane: %p\n", plane_pointer);

    this->plane_draw = Drawable(plane_pointer, position, rotation, the_scale);
    this->plane_draw.attachTextureSet(texture_set);

}

ParticleEmitter::ParticleEmitter(Drawable plane_draw){
    this->plane_draw = plane_draw;
}

ParticleEmitter::ParticleEmitter(glm::vec3 position, glm::vec3 initDir, int particleCount, float initialSpeed, float deceleration, float randomAmount, int decayTicks, GLuint shader_program){
    this->position = position;
    this->initDir = initDir;

    this->particleCount = particleCount;

    this->initialSpeed = initialSpeed;
    this->deceleration = deceleration;
    this->decayTicks = decayTicks;
    this->randomAmount = randomAmount;
    this->halfRandomAmount = randomAmount/2;
}

void ParticleEmitter::draw(glm::mat4* view_matrix, glm::mat4* proj_matrix, Light* light){

    #warning This may be a bottleneck: 7 calls to rand() every frame draw

    // glm::vec3 newPosition = position + glm::vec3(getRandomOffset(), getRandomOffset(), getRandomOffset());
    // glm::vec3 newInitDir  = initDir  + glm::vec3(getRandomOffset(), getRandomOffset(), getRandomOffset());

    // float newInitSpeed = initialSpeed + getRandomOffset();

    // Can grow up to a size of particleCount, but not bigger
    // recycling particle objects
    // if(particles.size() < particleCount){ 
    //     // Particle p(newPosition, newInitDir, newInitSpeed, deceleration, decayTicks, &plane);
    //     Particle p(position, initDir, initialSpeed, deceleration, decayTicks, &plane);
    //     particles.push_back(p);
    // } else {
    //     Particle front = particles[0];
    //     particles.pop_front();
    //     // front.setInitialValues(newPosition, newInitDir, newInitSpeed);
    //     front.setInitialValues(position, initDir, initialSpeed);
    //     particles.push_back(front);
    // }

    // for(int i(0); i < particles.size(); ++i){
        // particles[i].draw(view_matrix, proj_matrix, light);
    // }
    printf("plane: %p\n", plane_pointer);
    plane_draw.draw(view_matrix, proj_matrix, light);

}

float ParticleEmitter::getRandomOffset(){
    // Returns a value between -randomAmount/2 and +randomAmount/2.
    // This is to be added to the value you want randomized

    int scalar = (int)randomAmount*8;
    return (((float)(rand()%scalar)/8.0f) - halfRandomAmount);
}