// snow_emitter.h
// Trevor Westphal
#ifndef SnowEmitter_h
#define SnowEmitter_h

#include <glm/glm.hpp>

#include "emitter.h"
#include "camera.h"

class SnowEmitter : public Emitter {
public:
    SnowEmitter(GLuint, glm::vec3);
private:
    void prepareParticles(Camera*);
};

#endif