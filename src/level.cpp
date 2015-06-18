#include "level.hpp"

Level::Level(string filename, RenderDeque& render_stack) : unit_holder(), resource_loader(), game_map(filename, unit_holder, render_stack, resource_loader), unit_manager(game_map, unit_holder) {

    // Creation of test playable
# warning Move mesh loading into playable loading
    Mesh* playable_mesh = new Mesh("res/models/airship.dae");
    Shader& playable_shader_ref = resource_loader.loadShader("shaders/doodad.vs",
        "shaders/doodad.fs");
    float playable_scale = 1.0f;

    for(int i = 0; i < 1; ++i){
        for(int j = 0; j < 1; ++j){
            glm::vec3 playable_position = glm::vec3(-10 - 3.0f*i, 0.0f, 5 - 3.0f*j);
            Playable temp(playable_mesh, playable_shader_ref, playable_position, playable_scale);
            // temp.loadFromXML("res/units/testunit.xml");
            temp.loadFromXML("res/units/airship.xml");
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
