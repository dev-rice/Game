#include "emitter.h"

Emitter::Emitter(){
    printf("Created emitter: %p\n", this);
}

Emitter::Emitter(GLuint shader_program){
    printf("Created emitter: %p\n", this);

    GLfloat planeVerts[] = {-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                             0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                             0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,};

    GLuint  planeFaces[] = {0, 1, 2, 1, 2, 3};

    std::vector<GLfloat> planeVertsVector(planeVerts, planeVerts + sizeof(planeVerts) / sizeof(GLfloat));
    std::vector<GLuint> planeFacesVector(planeFaces, planeFaces + sizeof(planeFaces) / sizeof(GLuint));

    this->billboard = Mesh(planeVertsVector, planeFacesVector);
    this->billboard.attachShader(shader_program);
    
    this->particle = Drawable(&(this->billboard));
    TextureSet texture_set(0, 0, 0, 0);
    this->particle.attachTextureSet(texture_set);

    printf("     billboard = %p\n", &(this->billboard));
    printf("     particle  = %p\n", &(this->particle));

    


}

Emitter::Emitter(Mesh billboard, Drawable particle){
    this->billboard = billboard;
    this->particle = particle;

    printf("Created emitter: %p\n", this);
    printf("     billboard = %p\n", &(this->billboard));
    printf("     particle  = %p\n", &(this->particle));
}

void Emitter::draw(glm::mat4* view_matrix, glm::mat4* proj_matrix, Light* light){
    printf("Called draw on emitter at %p\n", this);
    printf("              billboard = %p\n", &(this->billboard));
    printf("              particle  = %p\n", &(this->particle));

    this->particle.draw(view_matrix, proj_matrix, light);
}
