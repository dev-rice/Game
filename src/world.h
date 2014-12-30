#ifndef World_h
#define World_h

#include <vector>
#include <random>
#include <math.h>

#include "level.h"
#include "game_view.h"
#include "debug.h"

class World{
public:
    World(GLFWwindow*);
    World(GLFWwindow*, const char*);
    ~World();

    void addLevel(const char*);
    void update();
    void handleInputs();

private:
    GLFWwindow* window;
    GameView* game_view;
    Level* level;
};

#endif
