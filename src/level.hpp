#ifndef Level_h
#define Level_h

#include "game_map.hpp"
#include "unit_manager.hpp"

using namespace std;

class Level {
public:
    Level(string filename, RenderStack& render_stack);

    GameMap& getGameMap();
    UnitManager& getUnitManager();

private:
    GameMap game_map;
    UnitManager unit_manager;
    UnitHolder unit_holder;
};


#endif
