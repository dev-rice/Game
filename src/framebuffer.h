#ifndef Framebuffer_h
#define Framebuffer_h

#include "flat_drawable.h"
#include "shader_loader.h"
#include "window.h"

class Framebuffer {
public:
    Framebuffer();
    Framebuffer(Window*);

    void setAsRenderTarget();
    void draw();
protected:

    GLuint framebuffer;
    GLuint framebuffer_texture;

    FlatDrawable* framebuffer_window;
};

#endif
