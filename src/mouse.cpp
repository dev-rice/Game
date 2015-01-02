#include "mouse.h"

Mouse::Mouse(FlatMesh* flat_mesh, GLFWwindow* window, GLuint shader_program, GLuint texture) 
        : UIDrawable(flat_mesh, window, shader_program, texture) {

    this->window = window;

    int window_width, window_height;
    glfwGetFramebufferSize(window, &window_width, &window_height);

    mouse_projection = glm::mat3( window_width / 2.0f, 0.0f, window_width / 2.0f,
                                  0.0f, -window_height / 2.0f, window_height / 2.0f,
                                  0.0f, 0.0f, 1.0f           );
}

glm::vec2 Mouse::getPosition(){
    double x;
    double y;
    glfwGetCursorPos(window, &x, &y);

    glm::vec3 gl_mouse_position = glm::vec3(x, y, 1.0) * glm::inverse(mouse_projection);

    return glm::vec2(gl_mouse_position.x, gl_mouse_position.y);
}

void Mouse::draw(){
    // Eww
    setPosition(getPosition());
    
    UIDrawable::draw();
}