#include "debug_console.h"

DebugConsole::DebugConsole(GLuint shader_program) : UIWindow(shader_program) {
    loadFromXML("res/layouts/debug_console.xml");
}
