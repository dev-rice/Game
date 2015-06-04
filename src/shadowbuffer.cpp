#include "shadowbuffer.h"


Shadowbuffer::Shadowbuffer(float up_sample){
    // Create frame buffer
    glGenFramebuffers(1, &framebuffer);

    setupFramebufferTexture(GL_RED, up_sample);

    //  Bind framebuffer and link texture to it
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
    framebuffer_texture.getGLId(), 0);

    setupDepthStencilBuffer();

    // Create the window to draw the framebuffer onto
    framebuffer_window = new FlatDrawable(0.25, 0.25, glm::vec2(0.75, -0.75));
    framebuffer_window->attachTexture(framebuffer_texture);
}

void Shadowbuffer::setAsRenderTarget(){
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, width, height);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
