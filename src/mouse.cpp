#include "mouse.h"

Mouse* Mouse::instance;

Mouse* Mouse::getInstance(){
    if(instance){
        return instance;
    } else {
        instance = new Mouse();
        return instance;
    }

}

Mouse::Mouse()
        : UIDrawable(ShaderLoader::loadShaderProgram("shaders/ui.vs","shaders/ui.fs"),
         TextureLoader::loadTextureFromFile("res/textures/cursor_ui.png", GL_LINEAR)) {


    int window_width = Window::getInstance()->getWidth();
    int window_height = Window::getInstance()->getHeight();

    inv_mouse_projection = glm::inverse(mouse_projection);

    // CURSOR, SELECTION, COMMAND, UP, RIGHT, DOWN, LEFT, UP_LEFT, UP_RIGHT, DOWN_RIGHT, DOWN_LEFT
    GLuint selection = TextureLoader::loadTextureFromFile("res/textures/cursor_select_ui.png", GL_NEAREST);
    GLuint command = TextureLoader::loadTextureFromFile("res/textures/cursor_select_ui.png", GL_NEAREST);
    GLuint up = TextureLoader::loadTextureFromFile("res/textures/cursor_scroll_up_ui.png", GL_NEAREST);
    GLuint right = TextureLoader::loadTextureFromFile("res/textures/cursor_scroll_right_ui.png", GL_NEAREST);
    GLuint down = TextureLoader::loadTextureFromFile("res/textures/cursor_scroll_down_ui.png", GL_NEAREST);
    GLuint left = TextureLoader::loadTextureFromFile("res/textures/cursor_scroll_left_ui.png", GL_NEAREST);
    GLuint up_left = TextureLoader::loadTextureFromFile("res/textures/cursor_scroll_up_left_ui.png", GL_NEAREST);
    GLuint up_right = TextureLoader::loadTextureFromFile("res/textures/cursor_scroll_up_right_ui.png", GL_NEAREST);
    GLuint down_right = TextureLoader::loadTextureFromFile("res/textures/cursor_scroll_down_right_ui.png", GL_NEAREST);
    GLuint down_left = TextureLoader::loadTextureFromFile("res/textures/cursor_scroll_down_left_ui.png", GL_NEAREST);

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

    hovering = false;

    // Set the cursor to the pointer by default
    setCursorSprite(Mouse::cursorType::CURSOR);
}

void Mouse::setCursorSprite(cursorType cursor_type){
    this->current_type = cursor_type;
}

void Mouse::draw(){
    glm::vec2 current_position = getGLPosition();

    // Position the cursor specially for certain cursors
    // mostly because down, right, up_right, down_right, and down_left run out of the screen
    // also selection needs to be moved to the center
    if(!hovering){
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
    } else {
        setGLPosition(current_position);
        current_type = cursorType::CURSOR;
    }

    attachTexture(mouse_sprites[ static_cast<int>(current_type) ]);

    width_pixels = 66;
    height_pixels = 66;
    updateDimensions();

    UIDrawable::draw();

    hovering = false;
}

glm::vec2 Mouse::getGLPosition(){
    glm::vec2 mouse_position = getScreenPosition();

    x_pixels = int(mouse_position.x) - 1;
    y_pixels = int(mouse_position.y) - 1;

    return UIDrawable::getGLPosition();
}

glm::vec2 Mouse::getScreenPosition(){
    sf::Window* sfml_window = Window::getInstance()->getSFMLWindow();
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(*sfml_window);
    return glm::vec2(mouse_pos.x, mouse_pos.y);
}

void Mouse::setHovering(){
    hovering = true;
}

bool Mouse::isHovering(){
    return hovering;
}
