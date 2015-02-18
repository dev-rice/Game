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

    graphics_menu = new UIWindow(ui_shader);
    graphics_menu->loadFromXML("res/layouts/graphics_settings.xml");
    graphics_menu->hide();

    toggle_key_state = false;
    debug_showing = Debug::is_on;

    menu_key_state = false;

    mouse_count = 0;
    left_mouse_button_unclick = false;

    attack_command_prime = false;

    // Usually OpenGL code shouldn't be this high up but this is for cool
    // mouse effects.
    glGenBuffers(1, &mouse_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, mouse_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 3, mouse_ubo, 0, sizeof(glm::vec3));

    Profile::getInstance()->updateShaderSettings();

}

void GameView::update(){
    // Tick the game clock.
    GameClock::getInstance()->tick();

    handleInputs();
    // Swap display/rendering buffers
    window->swapBuffers();


    // Render the shadow map into the shadow buffer
    if (Profile::getInstance()->isShadowsOn()){
        level->getShadowbuffer()->setAsRenderTarget();
        level->drawShadowMap();
    }

    // Render the level to the framebuffer
    if (Profile::getInstance()->isFramebuffersOn()){
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

    } else {
        // Draw the level
        screen->setAsRenderTarget();
        level->draw();
    }


    // Draw all of the ui elements on top of the level
    for(int i = 0; i < ui_drawables.size(); ++i){
        ui_drawables[i]->draw();
    }
    menu->draw();
    graphics_menu->draw();

    // draw selection rectangle here and change the cursor based on amount of dragging
    Camera* camera = level->getCamera();
    glm::mat4 proj_matrix = level->getProjection();
    glm::vec3 init = Mouse::getInstance()->getWorldPositionFromPoint(initial_left_click_position, camera, proj_matrix);
    glm::vec3 fina = Mouse::getInstance()->getWorldPositionFromPoint(final_left_click_position, camera, proj_matrix);

    bool dragged_x = fabs(initial_left_click_position.x - final_left_click_position.x) > 0.05;
    bool dragged_y = fabs(initial_left_click_position.y - final_left_click_position.y) > 0.05;

    if(mouse_count > 1 && !Mouse::getInstance()->isHovering() && (dragged_x || dragged_y)){
        // draw from initial_left_click_position to final_left_click_position
        Mouse::getInstance()->setCursorSprite(Mouse::cursorType::SELECTION);

        level->tempSelectUnits(init, fina);

        selection_box->setGLCoordinates(initial_left_click_position, final_left_click_position);
        selection_box->draw();
    }
    if(left_mouse_button_unclick && !Mouse::getInstance()->isHovering() && (dragged_x || dragged_y)){

        level->selectUnits(init, fina);

    } else if(left_mouse_button_unclick && !Mouse::getInstance()->isHovering()){
        level->selectUnit(Mouse::getInstance()->getWorldPosition(camera, proj_matrix));
    }

    // Draw the debug information
    if (debug_showing){
        Camera* camera = level->getCamera();
        glm::vec3 position = camera->getPosition();
        glm::vec3 rotation = camera->getRotation();
        glm::vec2 gl_mouse = Mouse::getInstance()->getGLPosition();

        float frame_time = GameClock::getInstance()->getDeltaTime();
        float average_frame_time = GameClock::getInstance()->getAverageDeltaTime();

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

    }

    // Calculating the mouse vector
    glm::vec3 mouse_point = Mouse::getInstance()->getWorldPosition(camera,
        proj_matrix);

    Terrain* terrain = level->getTerrain();
    mouse_point.y = terrain->getHeightInterpolated(mouse_point.x, mouse_point.z);

    glBindBuffer(GL_UNIFORM_BUFFER, mouse_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3),
        glm::value_ptr(mouse_point));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    DebugConsole::getInstance()->draw();

    // The mouse draws on top of everything else
    Mouse::getInstance()->draw();

}

void GameView::handleInputs(){
    Camera* camera = level->getCamera();
    glm::mat4 proj_matrix = level->getProjection();

    glfwPollEvents();

    glm::vec2 gl_mouse_position = Mouse::getInstance()->getGLPosition();

    if (debug_showing){
        // Camera controls for debug mode
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
    } else {
        // Mouse scrolling the screen when not in debug mode
        if(mouse_count == 0){

            Mouse::getInstance()->setCursorSprite(Mouse::cursorType::CURSOR);

            // LEFT
            if(camera->getPosition().x >= -1.0 * level->getMapWidth()/2 + 55){
                if(gl_mouse_position.x < -0.95){
                    camera->moveGlobalX(-10);
                } else if(gl_mouse_position.x < -0.85){
                    camera->moveGlobalX(-5);
                }
            }

            // RIGHT
            if(camera->getPosition().x <= 1.0 * level->getMapWidth()/2 - 55){
                if(gl_mouse_position.x > 0.95){
                    camera->moveGlobalX(10);
                } else if (gl_mouse_position.x > 0.85){
                    camera->moveGlobalX(5);
                }
            }

            // DOWN
            if(camera->getPosition().z <= 1.0 * level->getMapDepth()/2 - 3){
                if(gl_mouse_position.y < -0.95){
                    camera->moveGlobalZ(10);
                } else if(gl_mouse_position.y < -0.85){
                    camera->moveGlobalZ(5);
                }
            }

            // UP                            . Compensating for the camera angle
            if(camera->getPosition().z >= -1.0 * level->getMapDepth()/2 + 55){
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

    }
    // Closing the window
    if (glfwGetKey(glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        window->requestClose();
    }

    //##############################################################################
    // Shift Key Handling
    //##############################################################################
    bool shift_pressed = (glfwGetKey(glfw_window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS || glfwGetKey(glfw_window, GLFW_KEY_RIGHT_SHIFT)  == GLFW_PRESS);

    //##############################################################################
    // Left Mouse Button Handling
    //##############################################################################
    if(left_mouse_button_unclick){
        left_mouse_button_unclick = false;
    }

    if(glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_LEFT)){
        // Left mouse button
        if(attack_command_prime){

            attack_command_prime = false;
            level->issueOrder(Playable::Order::ATTACK, Mouse::getInstance()->getWorldPosition(camera, proj_matrix), shift_pressed);
            mouse_count = -1;
            left_mouse_button_unclick = true;

        } /* Probably more orders here */
        else if(mouse_count == 0){
            initial_left_click_position = gl_mouse_position;
        } else {
            final_left_click_position = gl_mouse_position;
        }
        mouse_count++;
    } else if(mouse_count != 0){
        mouse_count = 0;
        left_mouse_button_unclick = true;
    }

    //##############################################################################
    // Middle Mouse Button Handling
    //##############################################################################
    if(glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_MIDDLE)){
        // Middle mouse button
        if(!middle_mouse_button_click){
            level->issueOrder(Playable::Order::ATTACK, Mouse::getInstance()->getWorldPosition(camera, proj_matrix), shift_pressed);
        }

        attack_command_prime = false;
        right_mouse_button_click = true;
    } else if(middle_mouse_button_click){
        middle_mouse_button_click = false;
    }

    //##############################################################################
    // Right Mouse Button Handling
    //##############################################################################
    if(right_mouse_button_unclick){
        right_mouse_button_unclick = false;
    }

    if(glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_RIGHT)){
        // Right mouse button
        if(!right_mouse_button_click){

            level->issueOrder(Playable::Order::MOVE, Mouse::getInstance()->getWorldPosition(camera, proj_matrix), shift_pressed);
        }

        attack_command_prime = false;
        right_mouse_button_click = true;
    } else if(right_mouse_button_click){
        right_mouse_button_click = false;
        right_mouse_button_unclick = true;
    }

    //##############################################################################
    // Hold-Action Key Handling
    //##############################################################################
    if (glfwGetKey(glfw_window, GLFW_KEY_H) == GLFW_PRESS){
        level->issueOrder(Playable::Order::HOLD_POSITION, Mouse::getInstance()->getWorldPosition(camera, proj_matrix), shift_pressed);
    }

    //##############################################################################
    // Stop-Action Key Handling
    //##############################################################################
    if (glfwGetKey(glfw_window, GLFW_KEY_S) == GLFW_PRESS){
        level->issueOrder(Playable::Order::STOP, Mouse::getInstance()->getWorldPosition(camera, proj_matrix), shift_pressed);
    }

    //##############################################################################
    // Attack-Action Key Handling
    //##############################################################################
    if (glfwGetKey(glfw_window, GLFW_KEY_A) == GLFW_PRESS){
        attack_command_prime = true;
    }

    // Handle the debug toggle key
    if ((glfwGetKey(glfw_window, GLFW_KEY_TAB) == GLFW_PRESS) && (!toggle_key_state)){
        toggle_key_state = true;
        debug_showing = !debug_showing;
    }
    if (glfwGetKey(glfw_window, GLFW_KEY_TAB) == GLFW_RELEASE){
        toggle_key_state = false;
    }

    if (debug_showing){
        graphics_menu->show();
    } else {
        graphics_menu->hide();
    }

    // Handle the debug console toggle key
    if ((glfwGetKey(glfw_window, GLFW_KEY_F8) == GLFW_PRESS) && (!debug_console_key_state)){
        debug_console_key_state = true;
        DebugConsole::getInstance()->toggleShowing();
    }
    if (glfwGetKey(glfw_window, GLFW_KEY_F8) == GLFW_RELEASE){
        debug_console_key_state = false;
    }

    // Handle the menu toggle key
    if ((glfwGetKey(glfw_window, GLFW_KEY_F10) == GLFW_PRESS) && (!menu_key_state)){
        menu_key_state = true;
        menu->toggleShowing();
    }
    if (glfwGetKey(glfw_window, GLFW_KEY_F10) == GLFW_RELEASE){
        menu_key_state = false;
    }

    // Reset the average frame time calculations
    if (glfwGetKey(glfw_window, GLFW_KEY_T) == GLFW_PRESS){
        GameClock::getInstance()->resetAverage();
    }

    //Print the screen
    if ((glfwGetKey(glfw_window, GLFW_KEY_P) == GLFW_PRESS) && (!printscreen_key_state)){
        Window::getInstance()->takeScreenshot();
        printscreen_key_state = true;
    }
    if (glfwGetKey(glfw_window, GLFW_KEY_P) == GLFW_RELEASE){
        printscreen_key_state = false;
    }

}
