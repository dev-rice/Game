#include "level.hpp"

Level::Level(string filename) : unit_holder(), game_map(filename, unit_holder), unit_manager(game_map, unit_holder) {

    // Creation of test playable
    Mesh* playable_mesh = new Mesh("res/models/human.dae");
    GLuint playable_shader = ShaderLoader::loadShaderProgram("shaders/doodad.vs",
        "shaders/doodad.fs");
    float playable_scale = 1.0f;

    for(int i = 0; i < 10; ++i){
        for(int j = 0; j < 10; ++j){
            glm::vec3 playable_position = glm::vec3(3.0f*i, 0.0f, 3.0f*j);
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
