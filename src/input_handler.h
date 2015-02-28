#ifndef InputHandler_h
#define InputHandler_h

#include <functional>
#include <thread>
#include <SDL2/SDL.h>

#include "debug.h"
#include "window.h"

class InputHandler {
public:
    static InputHandler* getInstance();

    void pollInputs();
    void setCallback(std::function<void(SDL_Event)> callback);

private:
    InputHandler();

    void defaultCallback(SDL_Event);

    std::function<void(SDL_Event)> callback;
    std::thread polling_thread;

    static InputHandler* instance;

};

#endif
