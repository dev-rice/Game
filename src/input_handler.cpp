#include "input_handler.h"

InputHandler* InputHandler::instance;

InputHandler* InputHandler::getInstance(){
    if (instance){
        return instance;
    } else {
        instance = new InputHandler();
        return instance;
    }
}

InputHandler::InputHandler(){
    // Spawn the input polling thread
    polling_thread = std::thread(&InputHandler::pollInputs, this);
    std::function<void(SDL_Event)> callback_function = std::bind(&InputHandler::defaultCallback, this, std::placeholders::_1);
    setCallback(callback_function);
}

void InputHandler::pollInputs() {
    SDL_Event event;

    // Continue polling until the window has been closed.
    while (!Window::getInstance()->shouldClose()){
        while (SDL_PollEvent(&event)) {
            callback(event);
        }
    }
}

void InputHandler::setCallback(std::function<void(SDL_Event)> callback){
    Debug::info("Setting callback to %p.\n", &callback);
    this->callback = callback;
}

void InputHandler::defaultCallback(SDL_Event event){

}
