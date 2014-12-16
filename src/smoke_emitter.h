// smoke_emitter.h
// Trevor Westphal
#ifndef SmokeEmitter_h
#define SmokeEmitter_h

#include <glm/glm.hpp>

#include "emitter.h"
#include "camera.h"

class SmokeEmitter : public Emitter {
public:
    SmokeEmitter(GLuint, glm::vec3, float);
private:
    float radius;
    void prepareParticles(Camera*);
};  

#endif