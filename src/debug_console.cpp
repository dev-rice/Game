#include "debug_console.h"

DebugConsole* DebugConsole::instance;

DebugConsole* DebugConsole::getInstance(){
    if(instance){
        return instance;
    } else {
        instance = new DebugConsole();
        return instance;
    }

}

DebugConsole::DebugConsole() : UIWindow(ShaderLoader::loadShaderProgram("shaders/ui.vs","shaders/ui.fs")) {
    loadFromXML("res/layouts/debug_console.xml");
    text_renderer = new TextRenderer("res/fonts/inconsolata_bold_font.png", 22);

    // Do this so we don't have to process a ton of messages on the first
    // draw call
    syncWithDebug();

}

void DebugConsole::draw(){
    syncWithDebug();

    if (is_showing){
        UIWindow::draw();
        int start = 0;
        int end = messages.size() - 1;
        if (messages.size() > 16){
            start = end - 16;
        }
        for (int i = start; i <= end; ++i){
            const char* temp_buffer = messages[i].c_str();
            int x = x_pixels + 16;
            int y_base = y_pixels + height_pixels - 32;
            int y_offset = 22 * (end - i);
            int y = y_base - y_offset;
            text_renderer->print(x, y, temp_buffer);
        }
    }

}

void DebugConsole::clearMessages(){
    messages.clear();
}

void DebugConsole::syncWithDebug(){
    // Get all of the messages that debug has in its queue
    while (Debug::hasMessages()){
        std::string new_message = Debug::popMessage();
        messages.push_back(new_message);
    }
}
