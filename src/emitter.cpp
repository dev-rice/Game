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
    
    GLuint emit = TextureLoader::loadTextureFromFile("res/textures/ok.png", GL_NEAREST);

    texture_set = new TextureSet(0, 0, 0, emit);

    this->maxParticles = 100;
    this->shader_program = shader_program;


    #warning Particles do not load unless an initial one is put in the deque
    Particle* ptr = new Particle(billboard, texture_set, shader_program);
    particles.push_back(ptr);


}

Emitter::~Emitter(){
    // Fix me or something
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
    // glm::mat4 view_matrix = camera->getViewMatrix();
    // particle->draw(&view_matrix, proj_matrix, light);

    float r1 = (rand()%100)/float(100)-0.5f;
    float r2 = (rand()%100)/float(100)-0.76f;

    r1*=0.5f;
    r2*=0.5f;

    if(particles.size() < maxParticles){
        particles.push_back(new Particle(glm::vec3(-2.0f+r1, 0.0f+r2, 0.0f+r2), glm::vec3(0.0f+r2, 1.0f+r1, 0.0f+r2), 0.1f, 0.0015f, 50, billboard, texture_set, shader_program));
    } else {
        Particle* ptr = particles[0];
        particles.pop_front();
        ptr->setInitialValues(glm::vec3(-2.0f+r2, 0.0f+r1, 0.0f+r1), glm::vec3(0.0f+r2, 1.0f+r2, 0.0f+r1), 0.1f);
        particles.push_back(ptr);
    }

    for (int i = 0; i < particles.size(); ++i){
        particles[i]->draw(camera, proj_matrix, light);
    }
}