#ifndef RenderDeque_h
#define RenderDeque_h

#include <deque>

#include "screenbuffer.h"
#include "framebuffer.h"

class RenderDeque {
public:
    RenderDeque();

    void enqueueFramebuffer(Framebuffer& buf);
    void pushFramebuffer(Framebuffer& buf);
    void popFramebuffer();

    void drawAllToScreen();

private:

    std::deque<Framebuffer*> framebuffer_deuque;

    Screenbuffer screen;

};

#endif
