#ifndef DebugConsole_h
#define DebugConsole_h

#include <thread>
#include <mutex>

#include "core_ui/ui_window.h"
#include "debug.h"

class DebugConsole : public UIWindow {
public:
    static DebugConsole* getInstance();

    void draw();

private:
    DebugConsole();

    void pollInputs();

    TextRenderer* text_renderer;

    static DebugConsole* instance;

};

#endif
