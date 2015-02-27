#ifndef Shadowbuffer_h
#define Shadowbuffer_h

#include "framebuffer.h"
#include "core_ui/ui_window.h"
#include "profile.h"

class Shadowbuffer : public Framebuffer {
public:
    Shadowbuffer(float);

    void setAsRenderTarget();
    void draw();
};

#endif
