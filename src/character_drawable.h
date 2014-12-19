#ifndef CharacterDrawable_h
#define CharacterDrawable_h

#include <cmath>

#include "flat_drawable.h"
#include "character_mesh.h"

class CharacterDrawable : public FlatDrawable {
public:
    CharacterDrawable(FlatMesh*, GLuint, GLfloat, GLfloat, glm::vec2);

    void setCharacter(char);
private:
    void updateUniformData();

    glm::vec2 uv_offset;
};

#endif