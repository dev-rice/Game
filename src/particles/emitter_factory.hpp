#ifndef EmitterFactory_h
#define EmitterFactory_h

#include <string>

#include "all_emitters.hpp"
#include "../includes/json.hpp"
#include "debug.h"

using namespace std;

class EmitterFactory {
public:
    EmitterFactory() {;}

    Emitter* build(const Json::Value& emitter_json);
private:

};

#endif
