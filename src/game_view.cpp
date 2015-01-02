#include "game_view.h"

GameView::GameView(Window* window, Level* level){
    this->window = window;
    this->glfw_window = window->getGLFWWindow();
    this->level = level;

    screen = new Framebuffer();
    framebuffer = new Framebuffer(window);
    shadowbuffer = new Shadowbuffer(1024, 1024);

    FlatMesh* flat_mesh = new FlatMesh();

    GLuint ui_shader = ShaderLoader::loadShaderProgram("shaders/ui.vs",
        "shaders/ui.fs");
    GLuint mouse_texture = TextureLoader::loadTextureFromFile(
        "res/textures/cursor_ui.png", GL_LINEAR);

    mouse = new Mouse(flat_mesh, window, ui_shader, mouse_texture);

    text_renderer = new TextRenderer(window,
        "res/fonts/inconsolata_bold_font.png", 0.01);

    toggle_key_state = false;
    debug_showing = false;

    mouse_count = 0;
}

void GameView::update(){
    float start_time = glfwGetTime();
    // Swap display/rendering buffers
    window->swapBuffers();

    handleInputs();

    // Render the shadow map into the shadow buffer
    // shadowbuffer->setAsRenderTarget();

    // Render the level to the framebuffer
    framebuffer->setAsRenderTarget();
    // level->drawShadowMap();
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

    float frame_time = glfwGetTime() - start_time;

    // Draw the debug information
    if (debug_showing){
        Camera* camera = level->getCamera();
        glm::vec3 position = camera->getPosition();
        glm::vec3 rotation = camera->getRotation();
        glm::vec2 gl_mouse_position = mouse->getPosition();

        text_renderer->print(glm::vec2(-0.95, 0.95), "fps: %.2f",
            1.0 / frame_time);
        text_renderer->print(glm::vec2(-0.95, 0.9), "camera position <x, y, z>:"
            "%.2f, %.2f, %.2f", position.x, position.y, position.z);
        text_renderer->print(glm::vec2(-0.95, 0.85), "camera rotation <x, y, z>:"
            "%.2f, %.2f, %.2f", rotation.x, rotation.y, rotation.z);
        text_renderer->print(glm::vec2(-0.95, 0.80), "mouse <x, y>: %.2f, %.2f",
            gl_mouse_position.x, gl_mouse_position.y);
    }

    // draw selection rectangle here
    if(mouse_count > 0){
        // draw from initial_left_click_position to final_left_click_position
    }

    // The mouse draws on top of everything else
    mouse->draw();

}

void GameView::handleInputs(){
    Camera* camera = level->getCamera();

    glfwPollEvents();

    glm::vec2 gl_mouse_position = mouse->getPosition();
    if(gl_mouse_position.x < -0.9){
        camera->moveGlobalX(-5);
    } else if (gl_mouse_position.x > 0.9){
        camera->moveGlobalX(5);
    }

    if(gl_mouse_position.y < -0.9){
        camera->moveGlobalZ(5);
    } else if (gl_mouse_position.y > 0.9){
        camera->moveGlobalZ(-5);
    }

    if (glfwGetKey(glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(glfw_window, GL_TRUE);
    }

    if(glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_LEFT)){
        // Left mouse button
        if(mouse_count == 0){
            initial_left_click_position = gl_mouse_position;   
        } else {
            final_left_click_position = gl_mouse_position;
        }
        mouse_count++;
    } else if(mouse_count != 0){
        mouse_count = 0;
        // printf("Box from (%f, %f) to (%f, %f)\n", initial_right_click_position.x, initial_right_click_position.y, final_right_click_position.x, final_right_click_position.y);
    }

    if(glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_RIGHT)){
        // Right mouse button
        printf("Clicked right mouse button\n");
    }

    // Camera controls
    // Movement
    // if (glfwGetKey(glfw_window, GLFW_KEY_W) == GLFW_PRESS){
    //     camera->moveZ(-1);
    // }
    // if (glfwGetKey(glfw_window, GLFW_KEY_S) == GLFW_PRESS){
    //     camera->moveZ(1);
    // }
    // if (glfwGetKey(glfw_window, GLFW_KEY_D) == GLFW_PRESS){
    //     camera->moveX(1);
    // }
    // if (glfwGetKey(glfw_window, GLFW_KEY_A) == GLFW_PRESS){
    //     camera->moveX(-1);
    // }
    // if (glfwGetKey(glfw_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
    //     camera->moveY(-1);
    // }
    // if (glfwGetKey(glfw_window, GLFW_KEY_SPACE) == GLFW_PRESS){
    //     camera->moveY(1);
    // }

    // Rotation
    // if (glfwGetKey(glfw_window, GLFW_KEY_Q) == GLFW_PRESS){
    //     camera->rotateY(1);
    // }
    // if (glfwGetKey(glfw_window, GLFW_KEY_E) == GLFW_PRESS){
    //     camera->rotateY(-1);
    // }
    // if (glfwGetKey(glfw_window, GLFW_KEY_R) == GLFW_PRESS){
    //     camera->rotateX(1);
    // }
    // if (glfwGetKey(glfw_window, GLFW_KEY_F) == GLFW_PRESS){
    //     camera->rotateX(-1);
    // }
    // if (glfwGetKey(glfw_window, GLFW_KEY_Z) == GLFW_PRESS){
    //     camera->rotateZ(1);
    // }
    // if (glfwGetKey(glfw_window, GLFW_KEY_C) == GLFW_PRESS){
    //     camera->rotateZ(-1);
    // }

    // Handle the debug toggle key
    if ((glfwGetKey(glfw_window, GLFW_KEY_TAB) == GLFW_PRESS) && (!toggle_key_state)){
        toggle_key_state = true;
        debug_showing = !debug_showing;
    }
    if (glfwGetKey(glfw_window, GLFW_KEY_TAB) == GLFW_RELEASE){
        toggle_key_state = false;
    }

}
