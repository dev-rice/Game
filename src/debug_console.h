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
    void syncWithDebug();
    TextRenderer* text_renderer;
    std::vector<std::string> messages;

    static DebugConsole* instance;


};

#endif
