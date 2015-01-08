#ifndef Framebuffer_h
#define Framebuffer_h

#include "flat_drawable.h"
#include "shader_loader.h"
#include "window.h"

class Framebuffer {
public:
    Framebuffer();

    virtual void setAsRenderTarget();
    virtual void draw();
    GLuint getTexture() {return framebuffer_texture;}
protected:

    GLuint framebuffer;
    GLuint framebuffer_texture;

    FlatDrawable* framebuffer_window;
    Window* window;

    int width;
    int height;
};

#endif
