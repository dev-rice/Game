#include "emitter.h"

Emitter::Emitter(GLuint shader_program, glm::vec3 position){
    GLfloat planeVerts[] = {-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                             0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                             0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,};

    GLuint  planeFaces[] = {2, 1, 0, 1, 2, 3};

    this->position = position;

    std::vector<GLfloat> planeVertsVector(planeVerts, planeVerts + sizeof(planeVerts) / sizeof(GLfloat));
    std::vector<GLuint> planeFacesVector(planeFaces, planeFaces + sizeof(planeFaces) / sizeof(GLuint));

    billboard = new Mesh(planeVertsVector, planeFacesVector);

    GLuint emit = TextureLoader::loadTextureFromFile("res/textures/part_snow.png", GL_LINEAR);
    texture_set = new TextureSet(0, 0, 0, emit);

    this->maxParticles = 200;
    this->lifespan = 100;
    this->density = (this->maxParticles)/(this->lifespan);

    this->isShotgun = false;
    this->hasFired = false;

    this->shader_program = shader_program;
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
    // If the user does not explicitly set the density
    // using this function (thus creating different,
    // custom particle behavior), the emitter will
    // generate the density that will look best as per
    // the lifespan and the total number of particles
    // This will prevent live particles from being
    // recycled before it is their time
    this->density = density;
}

void Emitter::draw(Camera* camera, glm::mat4* proj_matrix){
    float new_time = glfwGetTime() - old_time;
    int frames = int((new_time*60.0f)) + 1;
    frames = std::max(frames, 0);

    glm::mat4 view_matrix = camera->getViewMatrix();

    for(int j = 0; j < frames; ++j){
        for(int i = 0; i < particles.size(); ++i){
            particles[i]->update();
        }
    }

    prepareParticles(camera);
    for(int i = 0; i < particles.size(); ++i){
        particles[i]->draw(&view_matrix, proj_matrix);
    }

    old_time = glfwGetTime();
}

void Emitter::makeShotgun(){
    density = maxParticles;
    isShotgun = true;
    hasFired = false;
}

void Emitter::prepareParticles(Camera* camera){

    // Shotgun stuff
    if(hasFired && isShotgun){
        return;
    }
    hasFired = true;

    for(int i(0); i < density; ++i){
        // Shitty random generation for now
        float rand1 = ((rand()%100)/100.0f)-0.5f;
        float rand2 = ((rand()%100)/100.0f)-0.5f;
        float rand3 = ((rand()%100)/100.0f)-0.5f;

        glm::vec3 position(-1.3f+rand1, 0.0f, rand2);
        // Emitter parented to camera
        // position+=camera->getPosition();
        glm::vec3 velocity(0.0f, 0.01f+(0.01f*rand3), 0.0f);
        glm::vec3 acceleration(0.0f, 0.0f, 0.0f);

        // Random wind interaction for snow particles.
        // if (rand() % 1000){
        //     velocity += glm::vec3(0.001 * (rand() % 5), - abs(0.0005 * (rand() % 5)), 0.001 * (rand() % 5));
        // }

        float rotation = 0.0f;

        // Particle recycling!
        // Weird that the pointer must be explicitly set to 0, but crashes without this
        Particle* ptr = 0;
        if(particles.size() < maxParticles){
            ptr = new Particle(billboard, shader_program);
            ptr->attachTextureSet(texture_set);
        }
        if(particles.size() > 0 && particles[0]->isDead()){
            ptr = particles[0];
            particles.pop_front();
        }
        if(ptr){
            ptr->setInitialValues(position, velocity, acceleration, rotation, lifespan, Particle::ScalingOption::SCALE_DOWN_WITH_AGE, Particle::FadingOption::FADE_NONE);
            particles.push_back(ptr);
        }
    }
}
