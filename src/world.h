#ifndef World_h
#define World_h

#include <vector>
#include <random>
#include <math.h>

#include "level.h"
#include "game_view.h"
#include "debug.h"
#include "window.h"

class World{
public:
    World(Window*);
    World(Window*, const char*);
    ~World();

    void addLevel(const char*);
    void update();
    void handleInputs();

private:
    Window* window;
    GLFWwindow* glfw_window;
    GameView* game_view;
    Level* level;
};

#endif
