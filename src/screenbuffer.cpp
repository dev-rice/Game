#include "screenbuffer.h"

Screenbuffer::Screenbuffer(Window* window){
    framebuffer = 0;
    width = window->getWidth();
    height = window->getHeight();
}
