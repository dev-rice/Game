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
    GLuint basic_vs = ShaderLoader::loadVertexShader("shaders/basic.vs");
    GLuint basic_fs = ShaderLoader::loadFragmentShader("shaders/basic.fs");
    basic_shader = ShaderLoader::combineShaderProgram(basic_vs, basic_fs);

    framebuffer_window = new FlatDrawable(flat_mesh, basic_shader, 0.25, 0.25, glm::vec2(0.75, -0.75));

    // Tell shader to read GL_TEXTURE0
    framebuffer_window->attachTexture(framebuffer_texture);

    flat_mesh = NULL;
    delete flat_mesh;

    text = new TextRenderer(window, "res/fonts/inconsolata_font.png", 0.05);


}

void GameView::update(){
    handleInputs();
    
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClearColor(0.0, 0.0, 0.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    level->draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0, 0.0, 0.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    level->draw();
    framebuffer_window->draw();
    
    text->drawString(glm::vec2(-0.9, 0.9), "Video Game.");


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

    camera = NULL;
    delete camera;
}

