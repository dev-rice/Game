#include "emitter.h"

Emitter::Emitter(GLuint shader_program){
    GLfloat planeVerts[] = {-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                             0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                             0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,};

    GLuint  planeFaces[] = {0, 1, 2, 1, 2, 3};

    std::vector<GLfloat> planeVertsVector(planeVerts, planeVerts + sizeof(planeVerts) / sizeof(GLfloat));
    std::vector<GLuint> planeFacesVector(planeFaces, planeFaces + sizeof(planeFaces) / sizeof(GLuint));

    billboard = new Mesh(planeVertsVector, planeFacesVector);
    billboard->attachShader(shader_program);
    
    texture_set = new TextureSet(0, 0, 0, 0);

    this->maxParticles = 100;

    particles.push_back(new Particle(glm::vec3(-2.2f, 0.0f, 0.0f), glm::vec3(-0.25f, 1.0f, 0.0f), 0.08f, 0.0015f, 50, billboard, texture_set));
    particles.push_back(new Particle(glm::vec3(-1.8f, 0.0f, 0.0f), glm::vec3(0.25f, 1.0f, 0.0f), 0.08f, 0.0015f, 50, billboard, texture_set));

}

Emitter::~Emitter(){
    billboard = NULL;
    delete billboard;
}

void Emitter::draw(glm::mat4* view_matrix, glm::mat4* proj_matrix, Light* light){

    float r1 = (rand()%100)/float(100)-0.5f;
    float r2 = (rand()%100)/float(100)-0.76f;

    r1*=0.5f;
    r2*=0.5f;

    if(particles.size() < maxParticles){
        particles.push_back(new Particle(glm::vec3(-2.0f+r1, 0.0f+r2, 0.0f+r2), glm::vec3(0.0f+r2, 1.0f+r1, 0.0f+r2), 0.1f, 0.0015f, 50, billboard, texture_set));
    } else {
        Particle* ptr = particles[0];
        particles.pop_front();
        ptr->setInitialValues(glm::vec3(-2.0f+r2, 0.0f+r1, 0.0f+r1), glm::vec3(0.0f+r2, 1.0f+r2, 0.0f+r1), 0.1f);
        particles.push_back(ptr);
    }

    for (int i = 0; i < particles.size(); ++i){
        particles[i]->draw(view_matrix, proj_matrix, light);
    }
}
