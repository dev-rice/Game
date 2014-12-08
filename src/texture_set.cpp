#include "texture_set.h"

TextureSet::TextureSet(GLuint diff, GLuint spec, GLuint norm, GLuint emit){

    GLuint pink = TextureLoader::loadPink();
    GLuint alpha = TextureLoader::loadAlpha();

    if (diff == 0){
        this->diffuse = pink;
        this->specular = pink;
        this->normal = pink;
        this->emissive = pink;
    }
    else {
        this->diffuse = diff;
        this->specular = spec;
        this->normal = norm;
        this->emissive = emit;
        if (spec == 0){
            this->specular = alpha;
        }
        if (norm == 0){
            this->normal = alpha;
        }
        if (emissive == 0){
            this->emissive = alpha;
        }
    }
    
}
