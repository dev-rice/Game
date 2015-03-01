#include "game_view.h"

GameView::GameView(Level* level){
    this->window = Window::getInstance();
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

    // Set the callback function to be the game view input
    InputHandler::Callback_Type callback_function = std::bind(&GameView::handleInput, this, std::placeholders::_1);
    InputHandler::getInstance()->pushCallback(callback_function);


}

void GameView::update(){
    // Tick the game clock.
    GameClock::getInstance()->tick();

    // Swap display/rendering buffers
    window->display();


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
        int fxaa_level = Profile::getInstance()->getFxaaLevel();
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

    // Calculating the mouse vector
    Camera* camera = level->getCamera();
    glm::mat4 proj_matrix = level->getProjection();
    Terrain* terrain = level->getTerrain();
    glm::vec3 mouse_point = level->calculateWorldPosition(Mouse::getInstance()->getGLPosition());

    // draw selection rectangle here and change the cursor based on amount of dragging
    glm::vec3 init = level->calculateWorldPosition(initial_left_click_position);
    glm::vec3 fina = level->calculateWorldPosition(final_left_click_position);

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
        level->selectUnit(level->calculateWorldPosition(Mouse::getInstance()->getGLPosition()));
    }

    // Draw the debug information
    if (debug_showing){
        Camera* camera = level->getCamera();
        glm::vec3 position = camera->getPosition();
        glm::vec3 rotation = camera->getRotation();

        float frame_time = GameClock::getInstance()->getDeltaTime();
        float average_frame_time = GameClock::getInstance()->getAverageDeltaTime();

        glm::vec2 mouse_gl_pos = Mouse::getInstance()->getGLPosition();
        glm::vec3 mouse_world_pos = level->calculateWorldPosition(mouse_gl_pos);

        text_renderer->print(10, 20, "fps: %.2f",
            1.0 / frame_time);
        text_renderer->print(10, 40, "average frame time: %.7f s",
            average_frame_time);
        text_renderer->print(10, 60, "camera position <x, y, z>:"
            "%.2f, %.2f, %.2f", position.x, position.y, position.z);
        text_renderer->print(10, 80, "camera rotation <x, y, z>:"
            "%.2f, %.2f, %.2f", rotation.x, rotation.y, rotation.z);
        text_renderer->print(10, 100, "mouse world position <x, y, z>:"
            "%.2f, %.2f, %.2f", mouse_world_pos.x, mouse_world_pos.y, mouse_world_pos.z);

    }

    // mouse_point.y = terrain->getHeightInterpolated(mouse_point.x, mouse_point.z);

    glBindBuffer(GL_UNIFORM_BUFFER, mouse_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3),
        glm::value_ptr(mouse_point));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    DebugConsole::getInstance()->draw();

    // The mouse draws on top of everything else
    Mouse::getInstance()->draw();

}

void GameView::handleInput(SDL_Event event){
    Camera* camera = level->getCamera();
    glm::mat4 proj_matrix = level->getProjection();
    Terrain* terrain = level->getTerrain();

    // Set the cursor to the pointer by default
    Mouse::getInstance()->setCursorSprite(Mouse::cursorType::CURSOR);

    // Get the mouse coordinates gl, and the world
    glm::vec2 mouse_gl_pos = Mouse::getInstance()->getGLPosition();
    glm::vec3 mouse_world_pos = level->calculateWorldPosition(mouse_gl_pos);

    SDL_Scancode key_scancode = event.key.keysym.scancode;
    switch(event.type){
        case SDL_KEYDOWN:
            if (key_scancode == SDL_SCANCODE_ESCAPE){
                window->requestClose();
            } else if (key_scancode == SDL_SCANCODE_T) {
                GameClock::getInstance()->resetAverage();
            } else if ((key_scancode == SDL_SCANCODE_TAB) && (!toggle_key_state)){
                toggle_key_state = true;
                debug_showing = !debug_showing;
            } else if ((key_scancode == SDL_SCANCODE_F8) && (!debug_console_key_state)){
                debug_console_key_state = true;
                DebugConsole::getInstance()->toggleShowing();
            } else if ((key_scancode == SDL_SCANCODE_G) && (!graphics_menu_key_state)){
                graphics_menu_key_state = true;
                graphics_menu->toggleShowing();
            } else if ((key_scancode == SDL_SCANCODE_F10) && (!menu_key_state)){
                menu_key_state = true;
                menu->toggleShowing();
            } else if ((key_scancode == SDL_SCANCODE_P) && (!printscreen_key_state)){
                printscreen_key_state = true;
                window->takeScreenshot();
            }
        break;

        case SDL_KEYUP:
            if (key_scancode == SDL_SCANCODE_TAB){
                toggle_key_state = false;
            } else if (key_scancode == SDL_SCANCODE_F8){
                debug_console_key_state = false;
            } else if (key_scancode == SDL_SCANCODE_G){
                graphics_menu_key_state = false;
            } else if (key_scancode == SDL_SCANCODE_F10){
                menu_key_state = false;
            } else if (key_scancode == SDL_SCANCODE_P){
                printscreen_key_state = false;
            }
        break;

    }

    SDL_PumpEvents();
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    //##############################################################################
    // Shift Key Handling
    //##############################################################################
    bool shift_pressed = state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT];

    //##############################################################################
    // Left Mouse Button Handling
    //##############################################################################
    if (left_mouse_button_unclick){
        left_mouse_button_unclick = false;
    }

    if (Mouse::getInstance()->isPressed(Mouse::LEFT)){
        // Left mouse button
        if (attack_command_prime){

            attack_command_prime = false;
            level->issueOrder(Playable::Order::ATTACK, mouse_world_pos, shift_pressed);
            mouse_count = -1;
            left_mouse_button_unclick = true;

        } /* Probably more orders here */
        else if (mouse_count == 0){
            initial_left_click_position = mouse_gl_pos;
        } else {
            final_left_click_position = mouse_gl_pos;
        }
        mouse_count++;
    } else if(mouse_count != 0){
        mouse_count = 0;
        left_mouse_button_unclick = true;
    }

    //##############################################################################
    // Middle Mouse Button Handling
    //##############################################################################
    if (Mouse::getInstance()->isPressed(Mouse::MIDDLE)){
        // Middle mouse button
        if (!middle_mouse_button_click){
            level->issueOrder(Playable::Order::ATTACK, mouse_world_pos, shift_pressed);
        }

        attack_command_prime = false;
        right_mouse_button_click = true;
    } else if (middle_mouse_button_click){
        middle_mouse_button_click = false;
    }

    //##############################################################################
    // Right Mouse Button Handling
    //##############################################################################
    if (right_mouse_button_unclick){
        right_mouse_button_unclick = false;
    }

    if (Mouse::getInstance()->isPressed(Mouse::RIGHT)){
        // Right mouse button
        if (!right_mouse_button_click){
            level->issueOrder(Playable::Order::MOVE, mouse_world_pos, shift_pressed);
        }

        attack_command_prime = false;
        right_mouse_button_click = true;

    } else if (right_mouse_button_click){
        right_mouse_button_click = false;
        right_mouse_button_unclick = true;
    }

    //##############################################################################
    // Hold-Action Key Handling
    //##############################################################################
    if (state[SDL_SCANCODE_H]){
        level->issueOrder(Playable::Order::HOLD_POSITION, mouse_world_pos, shift_pressed);
    }

    //##############################################################################
    // Stop-Action Key Handling
    //##############################################################################
    if (state[SDL_SCANCODE_S]){
        level->issueOrder(Playable::Order::STOP, mouse_world_pos, shift_pressed);
    }

    //##############################################################################
    // Attack-Action Key Handling
    //##############################################################################
    if (state[SDL_SCANCODE_A]){
        attack_command_prime = true;
    }
    //
    //##############################################################################
    // Camera Movement Handling
    //##############################################################################
    if (debug_showing){
        // Translation
        if (state[SDL_SCANCODE_W]){
            camera->moveZ(-1);
        }
        if (state[SDL_SCANCODE_S]){
            camera->moveZ(1);
        }
        if (state[SDL_SCANCODE_A]){
            camera->moveX(-1);
        }
        if (state[SDL_SCANCODE_D]){
            camera->moveX(1);
        }
        if (state[SDL_SCANCODE_LSHIFT]){
            camera->moveY(-1);
        }
        if (state[SDL_SCANCODE_SPACE]){
            camera->moveY(1);
        }

        // Rotation
        if (state[SDL_SCANCODE_E]){
            camera->rotateY(-1);
        }
        if (state[SDL_SCANCODE_Q]){
            camera->rotateY(1);
        }
        if (state[SDL_SCANCODE_F]){
            camera->rotateX(-1);
        }
        if (state[SDL_SCANCODE_R]){
            camera->rotateX(1);
        }
    } else {
        // Mouse scrolling the screen when not in debug mode
        if(mouse_count == 0){
            // LEFT
            if(camera->getPosition().x >= -1.0 * level->getMapWidth()/2 + 55){
                if(mouse_gl_pos.x < -0.95){
                    camera->moveGlobalX(-10);
                } else if(mouse_gl_pos.x < -0.85){
                    camera->moveGlobalX(-5);
                }
            }

            // RIGHT
            if(camera->getPosition().x <= 1.0 * level->getMapWidth()/2 - 55){
                if(mouse_gl_pos.x > 0.95){
                    camera->moveGlobalX(10);
                } else if (mouse_gl_pos.x > 0.85){
                    camera->moveGlobalX(5);
                }
            }

            // DOWN
            if(camera->getPosition().z <= 1.0 * level->getMapDepth()/2 - 3){
                if(mouse_gl_pos.y < -0.95){
                    camera->moveGlobalZ(10);
                } else if(mouse_gl_pos.y < -0.85){
                    camera->moveGlobalZ(5);
                }
            }

            // UP                            . Compensating for the camera angle
            if(camera->getPosition().z >= -1.0 * level->getMapDepth()/2 + 55){
                if(mouse_gl_pos.y > 0.95){
                    camera->moveGlobalZ(-10);
                } else if (mouse_gl_pos.y > 0.85){
                    camera->moveGlobalZ(-5);
                }
            }

            // Changing the mouse cursor based on scrolling
            if(mouse_gl_pos.x < -0.85){
                Mouse::getInstance()->setCursorSprite(Mouse::cursorType::LEFT);
            }
            if(mouse_gl_pos.x > 0.85){
                Mouse::getInstance()->setCursorSprite(Mouse::cursorType::RIGHT);
            }
            if(mouse_gl_pos.y > 0.85){
                Mouse::getInstance()->setCursorSprite(Mouse::cursorType::UP);
            }
            if(mouse_gl_pos.y < -0.85){
                Mouse::getInstance()->setCursorSprite(Mouse::cursorType::DOWN);
            }

            if(mouse_gl_pos.x < -0.85 && mouse_gl_pos.y < -0.85){
                Mouse::getInstance()->setCursorSprite(Mouse::cursorType::DOWN_LEFT);
            }
            if(mouse_gl_pos.x > 0.85 && mouse_gl_pos.y < -0.85){
                Mouse::getInstance()->setCursorSprite(Mouse::cursorType::DOWN_RIGHT);
            }
            if(mouse_gl_pos.x < -0.85 && mouse_gl_pos.y > 0.85){
                Mouse::getInstance()->setCursorSprite(Mouse::cursorType::UP_LEFT);
            }
            if(mouse_gl_pos.x > 0.85 && mouse_gl_pos.y > 0.85){
                Mouse::getInstance()->setCursorSprite(Mouse::cursorType::UP_RIGHT);
            }
        }

    }

}
