#ifndef Mouse_h
#define Mouse_h

#include "ui_drawable.h"

class Mouse : public UIDrawable {
public:
    Mouse(FlatMesh*, GLFWwindow*, GLuint, GLuint);

    void draw();

    glm::vec2 getPosition();

private:

    GLFWwindow* window;
    glm::mat3 mouse_projection;

};

#endif
