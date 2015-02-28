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
    // Set the default callback function
    Callback_Type callback_function = std::bind(&InputHandler::defaultCallback, this, std::placeholders::_1);
    pushCallback(callback_function);
}

void InputHandler::pollInputs() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        callbacks.top()(event);
    }
}

void InputHandler::pushCallback(Callback_Type& callback){
    callbacks.push(callback);
    Debug::info("Pushed callback %p to top of stack.\n", &callbacks.top());
}

void InputHandler::popCallback(){
    Debug::info("Popping callback %p off of the stack.\n", &callbacks.top());
    callbacks.pop();
}

void InputHandler::defaultCallback(SDL_Event event){

}
