#include "texture_set.h"

TextureSet::TextureSet(){
    load(0, 0, 0, 0);
}

TextureSet::TextureSet(GLuint diff, GLuint spec, GLuint norm, GLuint emit){
    load(diff, spec, norm, emit);
}

void TextureSet::load(GLuint diff, GLuint spec, GLuint norm, GLuint emit){
    GLuint pink = TextureLoader::loadPink();
    GLuint alpha = TextureLoader::loadAlpha();

    if (diff == 0){
        if (emit == 0){
            this->diffuse_set.push_back(pink);
            this->specular_set.push_back(pink);
            this->normal_set.push_back(pink);
            this->emissive_set.push_back(pink);
        } else {
            this->diffuse_set.push_back(alpha);
            this->specular_set.push_back(alpha);
            this->normal_set.push_back(alpha);
            this->emissive_set.push_back(emit);
        }

    }
    else {
        this->diffuse_set.push_back(diff);
        
        if (spec == 0){
            this->specular_set.push_back(alpha);
        } else {
            this->specular_set.push_back(spec);
        }

        if (norm == 0){
            this->normal_set.push_back(alpha);
        } else {
            this->normal_set.push_back(norm);
        }

        if (emit == 0){
            this->emissive_set.push_back(alpha);
        } else {
            this->emissive_set.push_back(emit);
        }
        
    }
}

GLuint TextureSet::getDiffuse(){
    return diffuse_set[0];
}

GLuint TextureSet::getSpecular(){
    return specular_set[0];
}

GLuint TextureSet::getNormal(){
    return normal_set[0];
}

GLuint TextureSet::getEmissive(){
    return emissive_set[0];
}