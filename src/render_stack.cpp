#include "render_stack.hpp"

RenderStack::RenderStack() : screen() {

}

void RenderStack::pushFramebuffer(Framebuffer& buf) {
    buf.setAsRenderTarget();
    framebuffer_stack.push(&buf);
}

void RenderStack::popFramebufferRendering() {
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

    // Render the things to the screen
    screen.setAsRenderTarget();

    bool is_first = true;
    while(!framebuffer_stack.empty()){
        if (is_first){
            // Temporarily disable blending for the first framebuffer. This ensures any alpha stuff in the first framebuffer doesn't blend with the default black background
            glDisable(GL_BLEND);
            getTop()->draw();
            glEnable(GL_BLEND);
            is_first = false;
        } else {
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            getTop()->draw();
            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        }

        // Regular pop without setting the next top one as the render target
        framebuffer_stack.pop();
    }
}
