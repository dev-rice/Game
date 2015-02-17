#include "debug_console.h"

std::mutex mtx;

DebugConsole::DebugConsole(GLuint shader_program) : UIWindow(shader_program) {
    loadFromXML("res/layouts/debug_console.xml");

    text_renderer = new TextRenderer("res/fonts/inconsolata_bold_font.png", 22);

}

void DebugConsole::draw(){
    if (is_showing){
        UIWindow::draw();

        std::vector<std::string> messages = Debug::getMessages();
        int start = 0;
        int end = messages.size() - 1;
        if (messages.size() > 16){
            start = end - 16;
        }
        for (int i = end; i >= start; --i){
            const char* temp_buffer = messages[i].c_str();
            int x = x_pixels + 16;
            int y = y_pixels + height_pixels - (22 * (i - start)) - 32;
            text_renderer->print(x, y, temp_buffer);
        }
    }

}
