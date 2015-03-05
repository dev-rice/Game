#ifndef Framebuffer_h
#define Framebuffer_h

#include <vector>

#include "flat_drawable.h"
#include "shader_loader.h"
#include "window.h"
#include "profile.h"

class Framebuffer {
public:
    Framebuffer();

    virtual void setAsRenderTarget();
    virtual void draw();

    void addShaderPass(GLuint);

    GLuint getTexture() {return framebuffer_texture;}
protected:

    GLuint framebuffer;
    GLuint framebuffer_texture;

    FlatDrawable* framebuffer_window;
    Window* window;

    std::vector<GLuint> shaders;

    int width;
    int height;
};

#endif
