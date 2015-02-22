#ifndef Mouse_h
#define Mouse_h

#include <vector>

#include "shader_loader.h"
#include "texture_loader.h"
#include "core_ui/ui_window.h"
#include "window.h"
#include "camera.h"
#include "terrain.h"

class Mouse : public UIDrawable {
public:
    static Mouse* getInstance();

    enum class cursorType { CURSOR, SELECTION, COMMAND, UP, RIGHT, DOWN, LEFT, UP_LEFT, UP_RIGHT, DOWN_RIGHT, DOWN_LEFT};

    void draw();
    void setCursorSprite(cursorType);
    void setHovering();
    bool isHovering();

private:

    bool hovering;

    static Mouse* instance;
    Mouse();

    cursorType current_type;

    std::vector<GLuint> mouse_sprites;

    GLFWwindow* glfw_window;
    glm::mat3 mouse_projection;
    glm::mat3 inv_mouse_projection;
};

#endif
