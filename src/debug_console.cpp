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
    input_buffer = "";
}

void DebugConsole::draw(){
    syncWithDebug();

    if (is_showing){
        UIWindow::draw();
        int start = 0;
        int end = messages.size() - 1;
        if (messages.size() > 15){
            start = end - 15;
        }
        for (int i = start; i <= end; ++i){
            const char* temp_buffer = messages[i].c_str();
            int x = x_pixels + 16;
            int y_base = y_pixels + height_pixels - 32 - (22);
            int y_offset = 22 * (end - i);
            int y = y_base - y_offset;
            text_renderer->print(x, y, temp_buffer);
        }

        // Print the input buffer
        text_renderer->print(x_pixels + 16, y_pixels + height_pixels - 32, input_buffer.c_str());

    }

}

void DebugConsole::show(){
    InputHandler::Callback_Type callback_function = std::bind(&DebugConsole::handleInput, this, std::placeholders::_1);
    InputHandler::getInstance()->pushCallback(callback_function);

    UIWindow::show();
}

void DebugConsole::hide(){
    InputHandler::getInstance()->popCallback();
    UIWindow::hide();
}

void DebugConsole::clearMessages(){
    messages.clear();
}

void DebugConsole::handleInput(SDL_Event event){
    switch (event.type) {
    case SDL_KEYUP:
        // Debug::info("Released: %s\n", SDL_GetKeyName(event.key.keysym.sym));
        break;
    case SDL_KEYDOWN:
        // Debug::info("Pressed: %s\n", SDL_GetKeyName(event.key.keysym.sym));
        input_buffer += SDL_GetKeyName(event.key.keysym.sym);
        if (event.key.keysym.scancode == SDL_SCANCODE_F8){
            hide();
        }
        break;
    case SDL_QUIT:
        Window::getInstance()->requestClose();
        break;
    }
}

void DebugConsole::syncWithDebug(){
    // Get all of the messages that debug has in its queue
    while (Debug::hasMessages()){
        std::string new_message = Debug::popMessage();
        messages.push_back(new_message);
    }
}
