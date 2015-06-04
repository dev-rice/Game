#ifndef RenderStack_h
#define RenderStack_h

#include <stack>

#include "screenbuffer.h"
#include "framebuffer.h"

class RenderStack {
public:
    RenderStack();

    void pushFramebuffer(Framebuffer& buf);
    void popFramebufferRendering();

    void drawAllToScreen();

private:

    Framebuffer* getTop();

    std::stack<Framebuffer*> framebuffer_stack;

    Screenbuffer screen;

};

#endif
