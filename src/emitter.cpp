#include "emitter.h"

Emitter::Emitter(GLuint shader_program){
    GLfloat planeVerts[] = {-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                             0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                             0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,};

    GLuint  planeFaces[] = {2, 1, 0, 1, 2, 3};

    std::vector<GLfloat> planeVertsVector(planeVerts, planeVerts + sizeof(planeVerts) / sizeof(GLfloat));
    std::vector<GLuint> planeFacesVector(planeFaces, planeFaces + sizeof(planeFaces) / sizeof(GLuint));

    billboard = new Mesh(planeVertsVector, planeFacesVector);
    
    // GLuint spec = TextureLoader::loadTextureFromFile("res/textures/bubble_spec.png", GL_NEAREST);
    GLuint emit = TextureLoader::loadTextureFromFile("res/textures/laser.png", GL_NEAREST);

    texture_set = new TextureSet(0, 0, 0, emit);

    this->maxParticles = 250;
    this->shader_program = shader_program;


    #warning Particles do not load unless an initial one is put in the deque
    Particle* ptr = new Particle(billboard, texture_set, shader_program);
    // particles.push_back(ptr);
}

Emitter::~Emitter(){
    // Deallocation is good!
    delete billboard;
    delete texture_set;

    texture_set = NULL;
    billboard = NULL;

    for(int i(0); i < particles.size(); ++i){
        delete particles[i];
        particles[i] = NULL;
    }
}

void Emitter::draw(Camera* camera, glm::mat4* proj_matrix, Light* light){

    glm::vec3 position(0.0f, 0.0f, 0.0f);
    glm::vec3 velocity(0.0f, 1.0f, 0.0f);
    glm::vec3 acceleration(0.0f, 0.0f, 0.0f);
    float rotation = 0.0f;
    int lifespan = 1000;

    // Recycle the particles instead of allocating new ones
    Particle* ptr;
    if(particles.size() < maxParticles){
        ptr = new Particle(billboard, texture_set, shader_program);
    } else {
        ptr = particles[0];
        particles.pop_front();
    }
    ptr->setInitialValues(position, velocity, acceleration, rotation, lifespan, Particle::ScalingOption::SCALE_NONE, Particle::FadingOption::FADE_NONE);
    particles.push_back(ptr);

    for (int i = 0; i < particles.size(); ++i){
        particles[i]->draw(camera, proj_matrix, light);
    }
}