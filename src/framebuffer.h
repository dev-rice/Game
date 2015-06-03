#ifndef Framebuffer_h
#define Framebuffer_h

#include <vector>

#include "flat_drawable.h"

#include "window.h"
#include "profile.h"

class Framebuffer {
public:
    Framebuffer();

    virtual void setAsRenderTarget();
    virtual void draw();

    void addShaderPass(Shader shader);

    Texture& getTexture() {return framebuffer_texture;}
protected:

    GLuint framebuffer;
    Texture framebuffer_texture;

    FlatDrawable* framebuffer_window;
    Window* window;

    std::vector<GLuint> shaders;

    int width;
    int height;
};

#endif
