#ifndef Mouse_h
#define Mouse_h

#include <vector>

#include "texture_loader.h"
#include "ui_drawable.h"
#include "window.h"

class Mouse : public UIDrawable {
public:
    enum class cursorType { CURSOR, SELECTION, COMMAND, UP, RIGHT, DOWN, LEFT, UP_LEFT, UP_RIGHT, DOWN_RIGHT, DOWN_LEFT};

    Mouse(FlatMesh*, GLuint, GLuint);

    void draw();
    void setCursorSprite(cursorType);

    glm::vec2 getPosition();

private:

    cursorType current_type;

    std::vector<GLuint> mouse_sprites;

    GLFWwindow* glfw_window;
    glm::mat3 mouse_projection;
    glm::mat3 inv_mouse_projection;
};

#endif
