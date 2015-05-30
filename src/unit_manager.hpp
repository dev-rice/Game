#ifndef UnitManager_h
#define UnitManager_h

#include "playable.h"
#include "unit_holder.hpp"
#include "game_map.hpp"

class UnitManager {
public:
    UnitManager(GameMap& game_map, std::vector<Playable*> units);

    void issueOrder(Playable::Order, glm::vec3, bool);
    void selectUnit(glm::vec3);
    void selectUnits(glm::vec3, glm::vec3);
    void tempSelectUnits(glm::vec3, glm::vec3);

private:
    float getDistance(float, float, float, float);

    std::vector<Playable*> all_units;
    std::vector<Playable*> selected_units;

    GameMap* game_map;

};

#endif
