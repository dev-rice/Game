// particle_emitter.cpp
// Trevor Westphal

#include "particle_emitter.h"

ParticleEmitter::ParticleEmitter(){
    this->position = glm::vec3(0.0f, 1.0f, 1.0f);
    this->initDir = glm::vec3(0.0f, 1.0f, 0.0f);

    this->particleCount = 1;
    this->initialSpeed = 1.0f;
    this->deceleration = 1.0f;
    this->decayTicks = 60;
    this->randomAmount = 0.0f;

    loadMesh();

    Particle p(position, initDir, initialSpeed, deceleration, decayTicks, &plane);
    this->particles.push_back(p);
}

ParticleEmitter::ParticleEmitter(glm::vec3 position, glm::vec3 initDir, int particleCount, float initialSpeed, float deceleration, float randomAmount, int decayTicks){
    this->position = position;
    this->initDir = initDir;

    this->particleCount = particleCount;

    this->initialSpeed = initialSpeed;
    this->deceleration = deceleration;
    this->decayTicks = decayTicks;
    this->randomAmount = randomAmount;
    this->halfRandomAmount = randomAmount/2;

    loadMesh();
}

void ParticleEmitter::loadMesh(){

    GLuint vertex_shader = ShaderLoader::loadVertexShader("shaders/vertex_shader.glsl");
    GLuint fragment_shader = ShaderLoader::loadFragmentShader("shaders/fragment_shader.glsl");
    GLuint shader_program = ShaderLoader::combineShaderProgram(vertex_shader, fragment_shader);

    GLfloat planeVerts[] = {0.5f,  0.5f, 0.0f,
                            0.5f, -0.5f, 0.0f, 
                           -0.5f,  0.5f, 0.0f, 
                           -0.5f, -0.5f, 0.0f};

    GLuint  planeFaces[] = {0, 1, 2, 1, 2, 3};

    std::vector<GLfloat> planeVertsVector(planeVerts, planeVerts + sizeof(planeVerts) / sizeof(GLfloat));
    std::vector<GLuint> planeFacesVector(planeFaces, planeFaces + sizeof(planeFaces) / sizeof(GLuint));
    // plane = Mesh(planeVertsVector, planeFacesVector);
    plane = Mesh("res/models/elbow.obj");
    plane.attachShader(shader_program);
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

    for(int i(0); i < particles.size(); ++i){
        particles[i].draw(view_matrix, proj_matrix, light);
    }

    // drawable.draw(view_matrix, proj_matrix, light);
}

float ParticleEmitter::getRandomOffset(){
    // Returns a value between -randomAmount/2 and +randomAmount/2.
    // This is to be added to the value you want randomized

    int scalar = (int)randomAmount*8;
    return (((float)(rand()%scalar)/8.0f) - halfRandomAmount);
}