#include "framebuffer.h"

Framebuffer::Framebuffer(){
    framebuffer = 0;
}

Framebuffer::Framebuffer(GLFWwindow* window){
    // Create a mesh for framebuffer to draw on
    FlatMesh* flat_mesh = new FlatMesh();

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

    // Load framebuffer shader
    GLuint framebuffer_vs = ShaderLoader::loadVertexShader("shaders/framebuffer.vs");
    GLuint framebuffer_fs = ShaderLoader::loadFragmentShader("shaders/framebuffer.fs");
    GLuint framebuffer_shader = ShaderLoader::combineShaderProgram(framebuffer_vs, framebuffer_fs);

    // Is slow for some reason
    // framebuffer_window = new FlatDrawable(flat_mesh, framebuffer_shader);
    framebuffer_window = new FlatDrawable(flat_mesh, framebuffer_shader, 1.0, 1.0, glm::vec2(0.0, 0.0));
    framebuffer_window->attachTexture(framebuffer_texture);

}

void Framebuffer::setAsRenderTarget(){
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClearColor(0.0, 0.0, 0.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::draw(){
    framebuffer_window->draw();
}
