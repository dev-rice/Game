#ifndef Doodad_h
#define Doodad_h

#include <iostream>

#include "drawable.h"
#include "includes/json.hpp"
#include "shader.hpp"

// using namespace std;

class Doodad: public Drawable {
public:
    Doodad(const Json::Value& doodad_json, std::string mesh_path, std::string texture_path);
    Doodad(Mesh*);
    Doodad(Mesh*, GLuint);
    Doodad(Mesh*, GLuint, glm::vec3, GLfloat);

private:
    void updateUniformData();

};

#endif
