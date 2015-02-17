#ifndef DebugConsole_h
#define DebugConsole_h

#include <thread>
#include <mutex>

#include "core_ui/ui_window.h"

class DebugConsole : public UIWindow {
public:
    DebugConsole(GLuint s);

    void draw();

private:
    void pollInputs(GLFWwindow* window, int key, int scancode, int action, int mods);

    TextRenderer* text_renderer;
    std::string input_buffer;

    std::thread input_thread;

};

#endif
