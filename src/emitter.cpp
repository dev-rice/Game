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

    particles.push_back(new Particle(glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 0.0015f, 50, billboard, texture_set));
    particles.push_back(new Particle(glm::vec3(-2.2f, 0.0f, 0.0f), glm::vec3(-0.25f, 1.0f, 0.0f), 0.08f, 0.0015f, 50, billboard, texture_set));
    particles.push_back(new Particle(glm::vec3(-1.8f, 0.0f, 0.0f), glm::vec3(0.25f, 1.0f, 0.0f), 0.08f, 0.0015f, 50, billboard, texture_set));

}

Emitter::~Emitter(){
    billboard = NULL;

    delete billboard;
}

void Emitter::draw(glm::mat4* view_matrix, glm::mat4* proj_matrix, Light* light){
    for (int i = 0; i < particles.size(); ++i){
        particles[i]->draw(view_matrix, proj_matrix, light);
    }
}
