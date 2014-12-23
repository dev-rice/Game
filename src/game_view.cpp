#include "game_view.h"

GameView::GameView(GLFWwindow* window, Level* level){
    this->window = window;
    this->level = level;

    // Create frame buffer
    glGenFramebuffers(1, &framebuffer);

    // Create texture for framebuffer
    glGenTextures(1, &framebuffer_texture);
    glBindTexture(GL_TEXTURE_2D, framebuffer_texture);
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //  Bind framebuffer and link texture to it
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer_texture, 0);

    // Add the depth buffer to the framebuffer
    GLuint rboDepthStencil;
    glGenRenderbuffers(1, &rboDepthStencil);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil);

    // Framebuffer Shtuff
    FlatMesh* flat_mesh = new FlatMesh();
    
    // Load framebuffer shader
    GLuint framebuffer_vs = ShaderLoader::loadVertexShader("shaders/framebuffer.vs");
    GLuint framebuffer_fs = ShaderLoader::loadFragmentShader("shaders/framebuffer.fs");
    GLuint framebuffer_shader = ShaderLoader::combineShaderProgram(framebuffer_vs, framebuffer_fs);

    framebuffer_window = new FlatDrawable(flat_mesh, framebuffer_shader, 0.25, 0.25, glm::vec2(0.75, -0.75));

    // Tell shader to read GL_TEXTURE0
    framebuffer_window->attachTexture(framebuffer_texture);

    GLuint ui_vs = ShaderLoader::loadVertexShader("shaders/ui.vs");
    GLuint ui_fs = ShaderLoader::loadFragmentShader("shaders/ui.fs");
    GLuint ui_shader = ShaderLoader::combineShaderProgram(ui_vs, ui_fs);
    GLuint mouse_texture = TextureLoader::loadTextureFromFile("res/textures/cursor_ui.png", GL_LINEAR);

    mouse = new UIDrawable(flat_mesh, window, ui_shader, mouse_texture);
    mouse_projection = glm::mat3( width / 2.0f, 0.0f         , width / 2.0f  ,
                                  0.0f        , -height / 2.0f, height / 2.0f,
                                  0.0f        , 0.0f         , 1.0f           );

    text_renderer = new TextRenderer(window, "res/fonts/inconsolata_bold_font.png", 0.01);

    toggle_key_state = false;
    debug_showing = false;
}

void GameView::update(){
    handleInputs();
    
    // Draw to the framebuffer
    // glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // glClearColor(0.0, 0.0, 0.0, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // level->draw();

    // Draw to the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0, 0.0, 0.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    level->draw();
    
    // Draw the framebuffer (currently a small window)
    // framebuffer_window->draw();
    
    if (debug_showing){
        Camera* camera = level->getCamera();
        glm::vec3 position = camera->getPosition();
        
        char buffer[100];
        sprintf(buffer, "camera <x, y, z>: %.2f, %.2f, %.2f", position.x, position.y, position.z);
        std::string to_print = buffer;
        text_renderer->drawString(glm::vec2(-0.95, 0.9), to_print);  
    }

    // Find the mouse position in gl coordinates
    double mouse_x;
    double mouse_y;

    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    glm::vec3 gl_mouse_position = glm::vec3(mouse_x, mouse_y, 1.0) * glm::inverse(mouse_projection);
    mouse->setPosition(glm::vec2(gl_mouse_position.x, gl_mouse_position.y));
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

    if ((glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) && (!toggle_key_state)){
        toggle_key_state = true;
        debug_showing = !debug_showing;
    }

    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE){
        toggle_key_state = false;
    }

    camera = NULL;
    delete camera;
}

