#ifndef DebugConsole_h
#define DebugConsole_h

#include <thread>
#include <mutex>

#include "core_ui/ui_window.h"
#include "debug.h"
#include "input_handler.h"
#include "level.h"

class DebugConsole : public UIWindow {
public:
    static DebugConsole* getInstance();

    void draw();
    void show();
    void hide();
    void clearMessages();

    void handleInput(SDL_Event);

    void setLevel(Level* level);

private:
    DebugConsole();

    void parseInput();
    void syncWithDebug();
    TextRenderer* text_renderer;
    std::vector<std::string> messages;
    std::string input_buffer;

    Level* level;

    static DebugConsole* instance;

};

#endif
