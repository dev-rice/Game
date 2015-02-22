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

    this->glfw_window = Window::getInstance()->getGLFWWindow();

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


}

void Mouse::setCursorSprite(cursorType cursor_type){
    this->current_type = cursor_type;
}

void Mouse::draw(){
    double x;
    double y;
    glfwGetCursorPos(glfw_window, &x, &y);

    x_pixels = int(x) - 1;
    y_pixels = int(y) - 1;

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

void Mouse::setHovering(){
    hovering = true;
}

bool Mouse::isHovering(){
    return hovering;
}

glm::vec3 getLinePlaneIntersection(glm::vec3 line_ray, glm::vec3 line_start, glm::vec3 plane_point, glm::vec3 plane_normal){
    // To find the point on the plane of clicking (defined by mouse_plane)
    // From http://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
    float d = glm::dot((plane_point - line_start), plane_normal) / glm::dot(line_ray, plane_normal);
    glm::vec3 intersection = d * line_ray + line_start;
    return intersection;
}

glm::vec3 Mouse::getWorldPositionFromPoint(glm::vec2 mouse_point, Camera* camera, glm::mat4& proj, Terrain* terrain){
    glm::vec3 world_mouse_point;

    glm::vec3 mouse_ray = getMouseRayFromPoint(mouse_point, camera, proj);

    // Search idea from http://bit.ly/1Jyb6pa
    int number_of_passes = 100;
    float max_height = terrain->getMaxHeight() + 1.0;
    float plane_increment = max_height / (float)number_of_passes;
    float distance_threshold = plane_increment;

    glm::vec3 line_start = camera->getPosition();
    glm::vec3 plane_normal = glm::vec3(0.0, 1.0, 0.0);

    for (int i = 0; i < number_of_passes; ++i){

        float plane_height = max_height - (i * plane_increment);
        glm::vec3 plane_point = glm::vec3(0.0, plane_height, 0.0);
        world_mouse_point = getLinePlaneIntersection(mouse_ray, line_start, plane_point, plane_normal);

        float terrain_height = terrain->getHeightInterpolated(world_mouse_point.x, world_mouse_point.z);
        if (!(abs(world_mouse_point.y - terrain_height) > distance_threshold)){
            break;
        }
    }

    return world_mouse_point;
}

glm::vec3 Mouse::getMouseRayFromPoint(glm::vec2 mouse_point, Camera* camera, glm::mat4& proj){
    glm::mat4 view = camera->getViewMatrix();
    glm::vec2 gl_mouse = mouse_point;
    glm::vec3 mouse_ray = glm::vec3(glm::inverse(proj) *
        glm::vec4(gl_mouse, -1.0, 1.0));
    mouse_ray.z = -1.0;
    mouse_ray = glm::vec3(glm::inverse(view) *
        glm::vec4(mouse_ray, 0.0));
    mouse_ray = glm::normalize(mouse_ray);
    return mouse_ray;
}

glm::vec3 Mouse::getWorldPosition(Camera* camera, glm::mat4& proj, Terrain* terrain){
    glm::vec2 mouse_coords = getGLPosition();
    return getWorldPositionFromPoint(mouse_coords, camera, proj, terrain);
}
