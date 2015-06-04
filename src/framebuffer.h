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

    void setupFramebufferTexture(GLuint format, float up_sample); 
    void setupDepthStencilBuffer();

    GLuint framebuffer;
    Texture framebuffer_texture;

    FlatDrawable* framebuffer_window;
    Window* window;

    std::vector<GLuint> shaders;

    int width;
    int height;
};

#endif
