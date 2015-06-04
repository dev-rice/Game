#include "render_stack.hpp"

RenderStack* RenderStack::instance;

RenderStack::RenderStack() : screen() {

}

RenderStack* RenderStack::getInstance() {
    if (!instance){
        instance = new RenderStack();
    }
    return instance;
}

void RenderStack::pushFramebuffer(Framebuffer& buf) {
    buf.setAsRenderTarget();
    framebuffer_stack.push(&buf);
}

void RenderStack::popFramebuffer() {
    framebuffer_stack.pop();

    // Set the new top one as the render target
    if (!framebuffer_stack.empty()){
        getTop()->setAsRenderTarget();
    }
}

Framebuffer* RenderStack::getTop(){
    return framebuffer_stack.top();
}

void RenderStack::drawAllToScreen() {
    // Goes through the render stack and draws each framebuffer to the screen. Order is retained because its a stack.
    screen.setAsRenderTarget();
    while(!framebuffer_stack.empty()){
        getTop()->draw();
        framebuffer_stack.pop();
    }
}
