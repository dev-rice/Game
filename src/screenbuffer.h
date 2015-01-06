#ifndef Screenbuffer_h
#define Screenbuffer_h

#include "framebuffer.h"
#include "window.h"

class Screenbuffer : public Framebuffer {
public:
    Screenbuffer(Window*);
};

#endif
