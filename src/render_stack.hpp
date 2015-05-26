#ifndef RenderStack_h
#define RenderStack_h

#include <stack>

#include "screenbuffer.h"
#include "framebuffer.h"

class RenderStack {
public:
    static RenderStack* getInstance();

    void pushFramebuffer(Framebuffer* buf);
    void popFramebuffer();

    void drawAllToScreen();

private:
    RenderStack();
    void initialize();

    Framebuffer* getTop();

    std::stack<Framebuffer*> framebuffer_stack;

    static RenderStack* instance;

    Screenbuffer screen;

};

#endif
