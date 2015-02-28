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
    Callback_Type callback_function = std::bind(&InputHandler::defaultCallback, this, std::placeholders::_1);

    Debug::info("Setting callback to %p.\n", &callback_function);
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

void InputHandler::setCallback(Callback_Type& callback){
    Debug::info("Setting callback to %p.\n", &callback);
    this->callback = callback;

    callbacks.push_back(&callback);
    for (int i = 0; i < callbacks.size(); ++i){
        Debug::info("callbacks[%d] = %p\n", i, callbacks[i]);        
    }

}

void InputHandler::defaultCallback(SDL_Event event){

}
