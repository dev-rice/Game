#include "texture_set.h"

TextureSet::TextureSet(GLint diffuse, GLint specular, GLint normal, GLint emissive){
    #warning set default textures
    if (diffuse != -1){
        this->diffuse = diffuse;
    }
    if (specular != -1){
        this->specular = specular;
    }
    if (normal != -1){
        this->normal = normal;
    }
    if (emissive != -1){
        this->emissive = emissive;
    }

}
