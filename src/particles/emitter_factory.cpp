#include "emitter_factory.hpp"

Emitter* EmitterFactory::build(const Json::Value& emitter_json){
    Emitter* emitter = NULL;

    string emitter_type = emitter_json["type"].asString();

    glm::vec3 position = glm::vec3(0, 0, 0);
    float radius = 0.7;

    if (emitter_type == "fire"){
        emitter = new FireEmitter(position, radius);
    } else if (emitter_type == "snow"){
        emitter = new SnowEmitter(position);
    } else if (emitter_type == "smoke"){
        emitter = new SmokeEmitter(position, radius);
    }

    return emitter;

}
