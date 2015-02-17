#ifndef DebugConsole_h
#define DebugConsole_h

#include <thread>
#include <mutex>

#include "core_ui/ui_window.h"
#include "debug.h"

class DebugConsole : public UIWindow {
public:
    DebugConsole(GLuint s);

    void draw();

private:
    void pollInputs();

    TextRenderer* text_renderer;

};

#endif
