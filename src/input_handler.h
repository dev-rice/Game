#ifndef InputHandler_h
#define InputHandler_h

#include <functional>
#include <thread>
#include <SDL2/SDL.h>
#include <stack>

#include "debug.h"
#include "window.h"


class InputHandler {
public:
    typedef std::function<void(SDL_Event)> Callback_Type;
    typedef std::function<void(void)> State_Callback_Type;

    static InputHandler* getInstance();

    void pollInputs();
    void pushCallback(Callback_Type& callback);
    void popCallback();

    void setStateCallback(State_Callback_Type callback);

private:
    InputHandler();

    void defaultCallback(SDL_Event);
    void defaultStateCallback();

    std::stack<Callback_Type> callbacks;
    State_Callback_Type state_callback;

    static InputHandler* instance;

};

#endif
