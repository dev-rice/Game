#include "shadowbuffer.h"

Shadowbuffer::Shadowbuffer(){
    // Create a mesh for framebuffer to draw on
    FlatMesh* flat_mesh = new FlatMesh();

    // Create frame buffer
    glGenFramebuffers(1, &framebuffer);

    // Create texture for framebuffer
    glGenTextures(1, &framebuffer_texture);
    glBindTexture(GL_TEXTURE_2D, framebuffer_texture);

    glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 1024, 1024, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);    
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, framebuffer_texture, 0);

    glDrawBuffer(GL_NONE); // No color buffer is drawn to.

    GLuint framebuffer_vs, framebuffer_fs, framebuffer_shader;
    // Load framebuffer shader
    framebuffer_vs = ShaderLoader::loadVertexShader("shaders/framebuffer.vs");
    framebuffer_fs = ShaderLoader::loadFragmentShader("shaders/framebuffer.fs");
    framebuffer_shader = ShaderLoader::combineShaderProgram(framebuffer_vs, framebuffer_fs);

    // Create the window to draw the framebuffer onto
    framebuffer_window = new FlatDrawable(flat_mesh, framebuffer_shader, 1.0, 1.0, glm::vec2(0.0, 0.0));
    framebuffer_window->attachTexture(framebuffer_texture);
}
