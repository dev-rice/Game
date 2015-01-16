// fire_emitter.h
// Trevor Westphal
#ifndef FireEmitter_h
#define FireEmitter_h

#include <glm/glm.hpp>

#include "particles/emitter.h"
#include "camera.h"

class FireEmitter : public Emitter {
public:
    FireEmitter(GLuint, glm::vec3, float);
private:
    float radius;
    void prepareParticles(Camera*);
};  

#endif