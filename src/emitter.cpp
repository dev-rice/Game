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
    GLuint emit = TextureLoader::loadTextureFromFile("res/textures/spark_part.png", GL_NEAREST);

    texture_set = new TextureSet(0, 0, 0, emit);

    this->maxParticles = 1000;
    this->lifespan = 200;
    this->density = (this->maxParticles)/(this->lifespan);

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

void Emitter::setParticleDensity(int density){
    this->density = density;
}

void Emitter::draw(Camera* camera, glm::mat4* proj_matrix, Light* light){
    prepareParticles();
    for (int i = 0; i < particles.size(); ++i){
        particles[i]->draw(camera, proj_matrix, light);
    }
}

void Emitter::prepareParticles(){
    for(int i(0); i < density; ++i){
        float rand1 = (rand()%100)/100.0f -0.5f;
        float rand2 = (rand()%100)/100.0f -0.5f;

        rand1*=0.2;
        rand2*=0.2;

        glm::vec3 position(rand1/2.0f, 0.0f, rand2/2.0f);
        glm::vec3 velocity(rand1, 0.1f, rand2);
        glm::vec3 acceleration(0.0f, -0.001f, 0.0f);
        float rotation = 0.0f;
        
        // Recycle the particles instead of allocating new ones
        Particle* ptr = 0; // Weird that the pointer must be explicitly set to 0, but crashes without this
        if(particles.size() < maxParticles){
            ptr = new Particle(billboard, texture_set, shader_program);
        } 
        if(particles.size() > 0 && particles[0]->isDead()){
            ptr = particles[0];
            particles.pop_front();
        }
        if(ptr){
            ptr->setInitialValues(position, velocity, acceleration, rotation, lifespan, Particle::ScalingOption::SCALE_NONE, Particle::FadingOption::FADE_NONE);
            particles.push_back(ptr);
        }
    }
}