#include "framebuffer.h"

Framebuffer::Framebuffer(){
    this->window = Window::getInstance();

    // Create frame buffer
    glGenFramebuffers(1, &framebuffer);

    // Create texture for framebuffer
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    width = window->getWidth();
    height = window->getHeight();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
        GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //  Bind framebuffer and link texture to it
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        texture_id, 0);

    // Add the depth buffer to the framebuffer
    GLuint rboDepthStencil;
    glGenRenderbuffers(1, &rboDepthStencil);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER, rboDepthStencil);

    Shader framebuffer_shader;
    // Load framebuffer shader
    if (Profile::getInstance()->getFxaaLevel()){
        framebuffer_shader = Shader("shaders/flat_drawable_noflip.vs",
            "shaders/framebuffer_fxaa.fs");
    } else {
        framebuffer_shader = Shader("shaders/flat_drawable_noflip.vs",
            "shaders/flat_drawable.fs");
    }

    addShaderPass(framebuffer_shader);

    // Create the window to draw the framebuffer onto
    framebuffer_texture = Texture(texture_id);
    framebuffer_window = new FlatDrawable(framebuffer_shader, 1.0, 1.0, glm::vec2(0.0, 0.0));
    framebuffer_window->attachTexture(framebuffer_texture);

}

void Framebuffer::setAsRenderTarget(){
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::draw(){
    // If this is an empty framebuffer then don't draw it
    if (framebuffer_window != NULL){
        // Do a draw pass for each of the shaders.
        for (GLuint shader : shaders){
            framebuffer_window->setShader(shader);
            framebuffer_window->draw();
        }
    }
}

void Framebuffer::addShaderPass(Shader shader){
    // Add a shader to the drawing passes. The pass
    // is done in the same order as the shaders were
    // added in
    shaders.push_back(shader.getGLId());
}
