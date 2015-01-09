#include "mouse.h"

Mouse::Mouse(GLuint shader_program, GLuint texture)
        : UIDrawable(shader_program, texture) {

    this->glfw_window = Window::getInstance()->getGLFWWindow();

    int window_width = Window::getInstance()->getWidth();
    int window_height = Window::getInstance()->getHeight();

    inv_mouse_projection = glm::inverse(mouse_projection);

    #warning standardize this shit!

    // CURSOR, SELECTION, COMMAND, UP, RIGHT, DOWN, LEFT, UP_LEFT, UP_RIGHT, DOWN_RIGHT, DOWN_LEFT
    GLuint selection = TextureLoader::loadTextureFromFile("res/textures/cursor_select_ui.png", GL_LINEAR);
    GLuint command = TextureLoader::loadTextureFromFile("res/textures/cursor_select_ui.png", GL_LINEAR);
    GLuint up = TextureLoader::loadTextureFromFile("res/textures/cursor_scroll_top_ui.png", GL_LINEAR);
    GLuint right = TextureLoader::loadTextureFromFile("res/textures/cursor_scroll_right_ui.png", GL_LINEAR);
    GLuint down = TextureLoader::loadTextureFromFile("res/textures/cursor_scroll_down_ui.png", GL_LINEAR);
    GLuint left = TextureLoader::loadTextureFromFile("res/textures/cursor_scroll_left_ui.png", GL_LINEAR);
    GLuint up_left = TextureLoader::loadTextureFromFile("res/textures/cursor_scroll_up_left_ui.png", GL_LINEAR);
    GLuint up_right = TextureLoader::loadTextureFromFile("res/textures/cursor_scroll_up_right_ui.png", GL_LINEAR);
    GLuint down_right = TextureLoader::loadTextureFromFile("res/textures/cursor_scroll_down_right_ui.png", GL_LINEAR);
    GLuint down_left = TextureLoader::loadTextureFromFile("res/textures/cursor_scroll_down_left_ui.png", GL_LINEAR);

    mouse_sprites.push_back(texture);
    mouse_sprites.push_back(selection);
    mouse_sprites.push_back(command);
    mouse_sprites.push_back(up);
    mouse_sprites.push_back(right);
    mouse_sprites.push_back(down);
    mouse_sprites.push_back(left);
    mouse_sprites.push_back(up_left);
    mouse_sprites.push_back(up_right);
    mouse_sprites.push_back(down_right);
    mouse_sprites.push_back(down_left);
}

void Mouse::setCursorSprite(cursorType cursor_type){
    this->current_type = cursor_type;
}

void Mouse::draw(){
    attachTexture(mouse_sprites[ static_cast<int>(current_type) ]);

    double x;
    double y;
    glfwGetCursorPos(glfw_window, &x, &y);

    x_pixels = int(x);
    y_pixels = int(y);

    glm::vec2 current_position = getGLPosition();

    // Position the cursor specially for certain cursors
    // mostly because down, right, up_right, down_right, and down_left run out of the screen
    // also selection needs to be moved to the center
    if(current_type == cursorType::DOWN || current_type == cursorType::DOWN_LEFT){
        setGLPosition(current_position + glm::vec2(0.0, 2*height));
    } else if(current_type == cursorType::RIGHT || current_type == cursorType::UP_RIGHT){
        setGLPosition(current_position + glm::vec2(-2*width, 0.0));
    } else if(current_type == cursorType::DOWN_RIGHT){
        setGLPosition(current_position + glm::vec2(-2*width, 2*height));
    } else if(current_type == cursorType::SELECTION){
        setGLPosition(current_position + glm::vec2(-1*width, height));
    } else {
        setGLPosition(current_position);
    }

    UIDrawable::draw();
}
