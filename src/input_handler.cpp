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
}

void InputHandler::pollInputs() {
    SDL_Event event;

    // Continue polling until the window has been closed.
    while (!Window::getInstance()->shouldClose()){
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYUP:
                Debug::info("Released: %s\n", SDL_GetKeyName(event.key.keysym.sym));
                break;
            case SDL_KEYDOWN:
                Debug::info("Pressed: %s\n", SDL_GetKeyName(event.key.keysym.sym));
                break;
            case SDL_QUIT:
                Window::getInstance()->requestClose();
                break;
            }
        }
    }
}

void InputHandler::setCallback(std::function<void(SDL_Event)> callback){
    this->callback = callback;
}
