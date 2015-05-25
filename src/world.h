#ifndef World_h
#define World_h

#include <vector>
#include <random>
#include <math.h>

#include "level.h"
#include "game_view.h"
#include "game_view_edit.h"
#include "debug.h"
#include "window.h"

class World{
public:
    World(bool);
    World(const char*, bool);
    ~World();

    void addLevel(const char*);
    void update();

private:
    Window* window;
    GameView* game_view;
    Level* level;

    bool edit_mode;
};

#endif
