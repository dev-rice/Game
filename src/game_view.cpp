#include "game_view.h"

GameView::GameView(Level* level){
    this->window = Window::getInstance();
    this->glfw_window = window->getGLFWWindow();
    this->level = level;

    screen = new Screenbuffer();
    framebuffer = new Framebuffer();

    GLuint ui_shader = ShaderLoader::loadShaderProgram("shaders/ui.vs",
        "shaders/ui.fs");
    GLuint mousebox_shader = ShaderLoader::loadShaderProgram("shaders/mousebox.vs",
        "shaders/mousebox.fs");

    // Unnecessary, but good to do
    Mouse::getInstance();

    text_renderer = new TextRenderer("res/fonts/inconsolata_bold_font.png", 20);

    // Creation of selection box
    selection_box = new UIDrawable(mousebox_shader, 0);
    selection_box->setOutline(true);

    // Creation of a test ui
    // For toggling the view state, the menu is better off for now as a pointer
    // independent of ui_drawables.
    // We should probably come up with a window manager class that keeps a list
    // of the currently showing UIWindows. This will be good for UIWindows with
    // sub windows.
    menu = new UIWindow(ui_shader);
    menu->loadFromXML("res/layouts/test.xml");
    menu->hide();

    toggle_key_state = false;
    debug_showing = Debug::is_on;

    menu_key_state = false;

    mouse_count = 0;

    frame_count = 0;
    average_frame_time = 0.0f;
}

void GameView::update(){
    ++frame_count;

    float start_time = glfwGetTime();
    // Swap display/rendering buffers
    window->swapBuffers();

    handleInputs();

    // Render the shadow map into the shadow buffer
    level->getShadowbuffer()->setAsRenderTarget();
    level->drawShadowMap();

    // Render the level to the framebuffer
    framebuffer->setAsRenderTarget();
    level->draw();

    // Draw the framebuffer N - 1 times (the last pass is drawn to the screen).
    // This is how many times the fxaa shader samples the image.
    // A good number is 4, 8 looks blurry, 1 doesn't do much.
    int fxaa_level = window->getFxaaLevel();
    if (fxaa_level){
        for (int i = 0; i < fxaa_level - 1; ++i){
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
    menu->draw();

    // draw selection rectangle here
    if(mouse_count > 1 && !Mouse::getInstance()->isHovering()){
        // draw from initial_left_click_position to final_left_click_position
        selection_box->setGLCoordinates(initial_left_click_position, final_left_click_position);
        selection_box->draw();
    }

    float frame_time = glfwGetTime() - start_time;

    // Calculate the moving average of the frame time
    average_frame_time = (average_frame_time *
        (float)(frame_count-1)/(float)frame_count)
        + (frame_time / (float)frame_count);

    // Draw the debug information
    if (debug_showing){
        Camera* camera = level->getCamera();
        glm::vec3 position = camera->getPosition();
        glm::vec3 rotation = camera->getRotation();
        glm::vec2 gl_mouse = Mouse::getInstance()->getGLPosition();


        glm::mat4 view_matrix = camera->getViewMatrix();
        glm::mat4 proj_matrix = level->getProjection();
        
        // Calculating the mouse vector
        glm::vec3 world_mouse = glm::vec3(glm::inverse(proj_matrix) *
            glm::vec4(gl_mouse, -1.0, 1.0));
        world_mouse.z = -1.0;
        world_mouse = glm::vec3(glm::inverse(camera->getViewMatrix()) *
            glm::vec4(world_mouse, 0.0));
        world_mouse = glm::normalize(world_mouse);

        text_renderer->print(10, 20, "fps: %.2f",
            1.0 / frame_time);
        text_renderer->print(10, 40, "average frame time: %.5f s",
            average_frame_time);
        text_renderer->print(10, 60, "camera position <x, y, z>:"
            "%.2f, %.2f, %.2f", position.x, position.y, position.z);
        text_renderer->print(10, 80, "camera rotation <x, y, z>:"
            "%.2f, %.2f, %.2f", rotation.x, rotation.y, rotation.z);
        text_renderer->print(10, 100, "mouse <x, y>: %.2f, %.2f",
            gl_mouse.x, gl_mouse.y);
        text_renderer->print(10, 120, "mouse vector <x, y, z>: %.2f, %.2f, %.2f",
            world_mouse.x, world_mouse.y, world_mouse.z);
    }

    // The mouse draws on top of everything else
    Mouse::getInstance()->draw();

}

void GameView::handleInputs(){
    Camera* camera = level->getCamera();

    glfwPollEvents();

    glm::vec2 gl_mouse_position = Mouse::getInstance()->getGLPosition();

    // Mouse scrolling the screen
    if(mouse_count == 0){

        Mouse::getInstance()->setCursorSprite(Mouse::cursorType::CURSOR);

        // LEFT
        if(camera->getPosition().x >= -1.0 * level->getMapWidth()/2 + 70){
            if(gl_mouse_position.x < -0.95){
                camera->moveGlobalX(-10);
            } else if(gl_mouse_position.x < -0.85){
                camera->moveGlobalX(-5);
            }
        }

        // RIGHT
        if(camera->getPosition().x <= 1.0 * level->getMapWidth()/2 - 70){
            if(gl_mouse_position.x > 0.95){
                camera->moveGlobalX(10);
            } else if (gl_mouse_position.x > 0.85){
                camera->moveGlobalX(5);
            }
        }

        // DOWN
        if(camera->getPosition().z <= 1.0 * level->getMapHeight()/2 - 3){
            if(gl_mouse_position.y < -0.95){
                camera->moveGlobalZ(10);
            } else if(gl_mouse_position.y < -0.85){
                camera->moveGlobalZ(5);
            }
        }

        // UP                            . Compensating for the camera angle
        if(camera->getPosition().z >= -1.0 * level->getMapHeight()/2 + 70){
            if(gl_mouse_position.y > 0.95){
                camera->moveGlobalZ(-10);
            } else if (gl_mouse_position.y > 0.85){
                camera->moveGlobalZ(-5);
            }
        }

        // Changing the mouse cursor based on scrolling
        if(gl_mouse_position.x < -0.85){
            Mouse::getInstance()->setCursorSprite(Mouse::cursorType::LEFT);
        }
        if(gl_mouse_position.x > 0.85){
            Mouse::getInstance()->setCursorSprite(Mouse::cursorType::RIGHT);
        }
        if(gl_mouse_position.y > 0.85){
            Mouse::getInstance()->setCursorSprite(Mouse::cursorType::UP);
        }
        if(gl_mouse_position.y < -0.85){
            Mouse::getInstance()->setCursorSprite(Mouse::cursorType::DOWN);
        }

        if(gl_mouse_position.x < -0.85 && gl_mouse_position.y < -0.85){
            Mouse::getInstance()->setCursorSprite(Mouse::cursorType::DOWN_LEFT);
        }
        if(gl_mouse_position.x > 0.85 && gl_mouse_position.y < -0.85){
            Mouse::getInstance()->setCursorSprite(Mouse::cursorType::DOWN_RIGHT);
        }
        if(gl_mouse_position.x < -0.85 && gl_mouse_position.y > 0.85){
            Mouse::getInstance()->setCursorSprite(Mouse::cursorType::UP_LEFT);
        }
        if(gl_mouse_position.x > 0.85 && gl_mouse_position.y > 0.85){
            Mouse::getInstance()->setCursorSprite(Mouse::cursorType::UP_RIGHT);
        }
    }

    // Closing the window
    if (glfwGetKey(glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(glfw_window, GL_TRUE);
    }

    if(glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_LEFT)){
        // Left mouse button
        if(mouse_count == 0){
            initial_left_click_position = gl_mouse_position;
        } else {
            final_left_click_position = gl_mouse_position;
            Mouse::getInstance()->setCursorSprite(Mouse::cursorType::SELECTION);
        }
        mouse_count++;
    } else if(mouse_count != 0){
        mouse_count = 0;
    }

    if(glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_RIGHT)){
        // Right mouse button
        // printf("Clicked right mouse button\n");
    }


    if (Debug::is_on){
        // Camera controls
        // Movement
        if (glfwGetKey(glfw_window, GLFW_KEY_W) == GLFW_PRESS){
            camera->moveZ(-1);
        }
        if (glfwGetKey(glfw_window, GLFW_KEY_S) == GLFW_PRESS){
            camera->moveZ(1);
        }
        if (glfwGetKey(glfw_window, GLFW_KEY_D) == GLFW_PRESS){
            camera->moveX(1);
        }
        if (glfwGetKey(glfw_window, GLFW_KEY_A) == GLFW_PRESS){
            camera->moveX(-1);
        }
        if (glfwGetKey(glfw_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
            camera->moveY(-1);
        }
        if (glfwGetKey(glfw_window, GLFW_KEY_SPACE) == GLFW_PRESS){
            camera->moveY(1);
        }

        // Rotation
        if (glfwGetKey(glfw_window, GLFW_KEY_Q) == GLFW_PRESS){
            camera->rotateY(1);
        }
        if (glfwGetKey(glfw_window, GLFW_KEY_E) == GLFW_PRESS){
            camera->rotateY(-1);
        }
        if (glfwGetKey(glfw_window, GLFW_KEY_R) == GLFW_PRESS){
            camera->rotateX(1);
        }
        if (glfwGetKey(glfw_window, GLFW_KEY_F) == GLFW_PRESS){
            camera->rotateX(-1);
        }
        if (glfwGetKey(glfw_window, GLFW_KEY_Z) == GLFW_PRESS){
            camera->rotateZ(1);
        }
        if (glfwGetKey(glfw_window, GLFW_KEY_C) == GLFW_PRESS){
            camera->rotateZ(-1);
        }
    }

    // Handle the debug toggle key
    if ((glfwGetKey(glfw_window, GLFW_KEY_TAB) == GLFW_PRESS) && (!toggle_key_state)){
        toggle_key_state = true;
        debug_showing = !debug_showing;
    }
    if (glfwGetKey(glfw_window, GLFW_KEY_TAB) == GLFW_RELEASE){
        toggle_key_state = false;
    }

    // Handle the menu toggle key
    if ((glfwGetKey(glfw_window, GLFW_KEY_F10) == GLFW_PRESS) && (!menu_key_state)){
        menu_key_state = true;
        if (menu->isShowing()){
            menu->hide();
        } else {
            menu->show();
        }
    }
    if (glfwGetKey(glfw_window, GLFW_KEY_F10) == GLFW_RELEASE){
        menu_key_state = false;
    }

    // Reset the average frame time calculations
    if (glfwGetKey(glfw_window, GLFW_KEY_T) == GLFW_PRESS){
        frame_count = 1;
        average_frame_time = 0.0f;
    }

    //Print the screen
    if (glfwGetKey(glfw_window, GLFW_KEY_P) == GLFW_PRESS){
        Window::getInstance()->takeScreenshot();
    }

}
