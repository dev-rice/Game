#include "game_view.h"

GameView::GameView(GLFWwindow* window, Level* level){
    this->window = window;
    this->level = level;

    screen = new Framebuffer();
    framebuffer = new Framebuffer(window);

    FlatMesh* flat_mesh = new FlatMesh();

    GLuint ui_vs = ShaderLoader::loadVertexShader("shaders/ui.vs");
    GLuint ui_fs = ShaderLoader::loadFragmentShader("shaders/ui.fs");
    GLuint ui_shader = ShaderLoader::combineShaderProgram(ui_vs, ui_fs);
    GLuint mouse_texture = TextureLoader::loadTextureFromFile("res/textures/cursor_ui.png", GL_LINEAR);

    mouse = new Mouse(flat_mesh, window, ui_shader, mouse_texture);

    text_renderer = new TextRenderer(window, "res/fonts/inconsolata_bold_font.png", 0.01);

    toggle_key_state = false;
    debug_showing = false;
}

void GameView::update(){
    handleInputs();

    // Render the level to the framebuffer
    framebuffer->setAsRenderTarget();
    level->draw();

    // Draw the framebuffer N - 1 times (the last pass is drawn to the screen).
    // This is how many times the fxaa shader samples the image.
    // A good number is 4, 8 looks blurry, 1 doesn't do much.
    if (Profile::fxaa){
        for (int i = 0; i < Profile::fxaa_level - 1; ++i){
            framebuffer->draw();
        }
    }
    // Draw the framebuffer
    screen->setAsRenderTarget();
    framebuffer->draw();

    // Draw all of the ui elements on top of the level
    for(int i = 0; i < ui_drawables.size(); ++i){
        ui_drawables[i]->draw();
    }

    // Draw the debug information
    if (debug_showing){
        Camera* camera = level->getCamera();
        glm::vec3 position = camera->getPosition();
        glm::vec3 rotation = camera->getRotation();
        glm::vec2 gl_mouse_position = mouse->getPosition();

        char buffer[100];
        std::string to_print;

        sprintf(buffer, "camera position <x, y, z>: %.2f, %.2f, %.2f", position.x, position.y, position.z);
        to_print = buffer;
        text_renderer->drawString(glm::vec2(-0.95, 0.9), to_print);

        sprintf(buffer, "camera rotation <x, y, z>: %.2f, %.2f, %.2f", rotation.x, rotation.y, rotation.z);
        to_print = buffer;
        text_renderer->drawString(glm::vec2(-0.95, 0.85), to_print);

        sprintf(buffer, "mouse <x, y>: %.2f, %.2f", gl_mouse_position.x, gl_mouse_position.y);
        to_print = buffer;
        text_renderer->drawString(glm::vec2(-0.95, 0.80), to_print);

        // Framerate stuff
        start_time = std::clock() - end_time;

        sprintf(buffer, "current framerate: %.2f", 0.01f*float(start_time));
        to_print = buffer;
        text_renderer->drawString(glm::vec2(-0.95, 0.75), to_print);

        end_time = std::clock();
    }

    // The mouse draws on top of everything else
    mouse->draw();

}

void GameView::handleInputs(){
    Camera* camera = level->getCamera();

    glfwPollEvents();

    // Camera controls
    // Movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera->moveZ(-1);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera->moveZ(1);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera->moveX(1);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera->moveX(-1);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        camera->moveY(-1);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        camera->moveY(1);
    }

    // Rotation
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        camera->rotateY(1);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        camera->rotateY(-1);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        camera->rotateX(1);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
        camera->rotateX(-1);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
        camera->rotateZ(1);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
        camera->rotateZ(-1);
    }

    // Handle the debug toggle key
    if ((glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) && (!toggle_key_state)){
        toggle_key_state = true;
        debug_showing = !debug_showing;
    }
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE){
        toggle_key_state = false;
    }

}
