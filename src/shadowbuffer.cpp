#include "shadowbuffer.h"

Shadowbuffer::Shadowbuffer(float up_sample){
    // Create a mesh for framebuffer to draw on
    FlatMesh* flat_mesh = new FlatMesh();

    // Create frame buffer
    glGenFramebuffers(1, &framebuffer);

    // Create texture for framebuffer
    glGenTextures(1, &framebuffer_texture);
    glBindTexture(GL_TEXTURE_2D, framebuffer_texture);

    this->width = up_sample * Window::getInstance()->getWidth();
    this->height = up_sample * Window::getInstance()->getHeight();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB,
        GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //  Bind framebuffer and link texture to it
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
    framebuffer_texture, 0);

    // Add the depth buffer to the framebuffer
    GLuint rboDepthStencil;
    glGenRenderbuffers(1, &rboDepthStencil);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->width,
        this->height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER, rboDepthStencil);

    GLuint framebuffer_vs, framebuffer_fs, framebuffer_shader;
    // Load framebuffer shader
    if (Window::getInstance()->getFxaaLevel()){
        framebuffer_shader = ShaderLoader::loadShaderProgram("shaders/framebuffer_fxaa.vs",
        "shaders/framebuffer_fxaa.fs");
    } else {
        framebuffer_shader = ShaderLoader::loadShaderProgram("shaders/framebuffer.vs",
        "shaders/framebuffer.fs");
    }

    // Create the window to draw the framebuffer onto
    framebuffer_window = new FlatDrawable(flat_mesh, framebuffer_shader, 0.25, 0.25, glm::vec2(0.75, -0.75));
    framebuffer_window->attachTexture(framebuffer_texture);
}

void Shadowbuffer::setAsRenderTarget(){
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, width, height);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Shadowbuffer::draw(){
    // If this is an empty framebuffer then don't draw it
    if (framebuffer_window != NULL){
        framebuffer_window->draw();
    }
}
