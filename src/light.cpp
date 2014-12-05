#include "light.h"

Light::Light(){

}

Light::Light(glm::vec3 position, glm::vec3 color, GLfloat intensity){
    this->position = position;
    this->color = color;
    this->intensity = intensity;
}

GLfloat* Light::getPosition(){
    return glm::value_ptr(position);
}

GLfloat* Light::getColor(){
    return glm::value_ptr(color);
}

GLfloat* Light::getIntensity(){
    return &intensity;
}