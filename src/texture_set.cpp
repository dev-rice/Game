#include "texture_set.h"

TextureSet::TextureSet(GLuint diffuse, GLuint specular, GLuint normal, GLuint emissive){
    this->diffuse = diffuse;
    this->specular = specular;
    this->normal = normal;
    this->emissive = emissive;
}
