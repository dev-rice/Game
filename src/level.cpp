#include "level.hpp"

Level::Level(string filename) : unit_holder(), game_map(filename, &unit_holder), unit_manager(game_map, unit_holder) {

}

GameMap& Level::getGameMap() {
    return game_map;
}

UnitManager& Level::getUnitManager() {
    return unit_manager;
}
