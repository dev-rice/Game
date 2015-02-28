#ifndef InputHandler_h
#define InputHandler_h

#include <functional>
#include <thread>
#include <SDL2/SDL.h>

#include "debug.h"
#include "window.h"


class InputHandler {
public:
    typedef std::function<void(SDL_Event)> Callback_Type;

    static InputHandler* getInstance();

    void pollInputs();
    void setCallback(Callback_Type& callback);

private:
    InputHandler();

    void defaultCallback(SDL_Event);

    Callback_Type callback;
    std::vector<Callback_Type*> callbacks;
    std::thread polling_thread;

    static InputHandler* instance;

};

#endif
