#ifndef CharacterDrawable_h
#define CharacterDrawable_h

#include <cmath>

#include "flat_drawable.h"
#include "character_mesh.h"
#include "core_ui/ui_drawable.h"

class CharacterDrawable : public UIDrawable {
public:
    CharacterDrawable(GLuint, GLuint, GLint);

    void setCharacter(char);
    void setPixelPosition(int, int);
    void moveToNext();
private:
    void updateUniformData();

    glm::vec2 uv_offset;
};

#endif
