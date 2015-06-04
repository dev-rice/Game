#include "level.hpp"

Level::Level(string filename, RenderStack& render_stack) : unit_holder(), game_map(filename, unit_holder, render_stack), unit_manager(game_map, unit_holder) {

    // Creation of test playable
    Mesh* playable_mesh = new Mesh("res/models/human.dae");
    Shader playable_shader("shaders/doodad.vs",
        "shaders/doodad.fs");
    float playable_scale = 1.0f;

    for(int i = 0; i < 5; ++i){
        for(int j = 0; j < 5; ++j){
            glm::vec3 playable_position = glm::vec3(5 - 3.0f*i, 0.0f, 5 - 3.0f*j);
            Playable temp(playable_mesh, playable_shader, playable_position, playable_scale);
            temp.loadFromXML("res/units/testunit.xml");
            temp.setScale(0.8);
            if (rand() % 2){
                temp.setTeam(1);
            } else {
                temp.setTeam(2);
            }

            unit_holder.addUnit(temp);
        }
    }

}

GameMap& Level::getGameMap() {
    return game_map;
}

UnitManager& Level::getUnitManager() {
    return unit_manager;
}
