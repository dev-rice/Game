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

    pushCallback(callback_function);
}

void InputHandler::pollInputs() {
    SDL_Event event;

    // Continue polling until the window has been closed.
    while (!Window::getInstance()->shouldClose()){
        while (SDL_PollEvent(&event)) {
            callbacks.top()(event);
        }
    }
}

void InputHandler::pushCallback(Callback_Type& callback){
    Debug::info("Pushing callback %p to top of stack.\n", &callback);
    callbacks.push(callback);
}

void InputHandler::popCallback(){
    callbacks.pop();
}

void InputHandler::defaultCallback(SDL_Event event){

}
