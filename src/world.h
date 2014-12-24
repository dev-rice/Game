#ifndef World_h
#define World_h

#include <vector>
#include <random>
#include <math.h>

#include "level.h"
#include "game_view.h"

class World{
public:
    World(GLFWwindow*);
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
