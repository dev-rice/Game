#ifndef Mouse_h
#define Mouse_h

#include "ui_drawable.h"
#include "window.h"

class Mouse : public UIDrawable {
public:
    Mouse(FlatMesh*, Window*, GLuint, GLuint);

    void draw();

    glm::vec2 getPosition();

private:

    GLFWwindow* glfw_window;
    glm::mat3 mouse_projection;

};

#endif
