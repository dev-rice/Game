#include "shadowbuffer.h"

Shadowbuffer::Shadowbuffer(Window* window, int width, int height){
    // Create a mesh for framebuffer to draw on
    FlatMesh* flat_mesh = new FlatMesh();

    // Create frame buffer
    glGenFramebuffers(1, &framebuffer);

    // Create texture for framebuffer
    glGenTextures(1, &framebuffer_texture);
    glBindTexture(GL_TEXTURE_2D, framebuffer_texture);

    glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, width, height, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, framebuffer_texture, 0);

    glDrawBuffer(GL_NONE); // No color buffer is drawn to.

    GLuint framebuffer_vs, framebuffer_fs, framebuffer_shader;
    // Load framebuffer shader
    framebuffer_shader = ShaderLoader::loadShaderProgram("shaders/framebuffer.vs",
        "shaders/framebuffer.fs");

    // Create the window to draw the framebuffer onto
    framebuffer_window = new UIDrawable(flat_mesh, window, framebuffer_shader,
        framebuffer_texture);
    framebuffer_window->setPosition(glm::vec2(0.25, 0.25));
}
