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
}

void GameView::update(){
    level->handleInputs();
    
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClearColor(0.0, 0.0, 0.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    level->draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    level->draw();
    framebuffer_window->draw();

}

