#ifndef CharacterDrawable_h
#define CharacterDrawable_h

#include <cmath>

#include "flat_drawable.h"
#include "character_mesh.h"
#include "core_ui/ui_drawable.h"
#include "font_sheet.h"
#include "glyph.h"

class CharacterDrawable : public UIDrawable {
public:
    CharacterDrawable(GLuint, FontSheet*, GLint);

    void setCharacter(char);
    void setPixelPosition(int, int);
    void setTextColor(glm::vec3 color);
    void setTextColor(float r, float g, float b);
    void setSpacing(float spacing);
    void moveToNext();
private:
    void updateUniformData();

    glm::vec2 uv_offset;
    glm::vec3 text_color;
    float spacing;

    int cursor_x;
    int cursor_y;

    FontSheet* font_sheet;
    Glyph current_glyph;
};

#endif
