// snow_emitter.h
// Trevor Westphal
#ifndef SnowEmitter_h
#define SnowEmitter_h

#include <glm/glm.hpp>

#include "particles/emitter.h"
#include "camera.h"

class SnowEmitter : public Emitter {
public:
    SnowEmitter(const Json::Value& emitter_json);
    SnowEmitter(glm::vec3);
    SnowEmitter(GLuint, glm::vec3);
private:
    void initialize();
    void prepareParticles(Camera*);
};

#endif
