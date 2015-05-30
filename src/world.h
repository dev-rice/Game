#ifndef World_h
#define World_h

#include <vector>
#include <random>
#include <math.h>

#include "game_view.h"
#include "game_view_edit.h"
#include "debug.h"
#include "window.h"

using namespace std;

class World{
public:
    World(string, bool);
    ~World();

    void addLevel(string);
    void update();

private:
    GameView* game_view;

    bool edit_mode;
};

#endif
