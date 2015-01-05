#ifndef Shadowbuffer_h
#define Shadowbuffer_h

#include "framebuffer.h"
#include "ui_drawable.h"

class Shadowbuffer : public Framebuffer {
public:
    Shadowbuffer(Window*, int, int);

    void setAsRenderTarget();
    void draw();
};

#endif
