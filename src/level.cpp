#include "level.hpp"

Level::Level(string filename, RenderDeque& render_stack) : File(filename), unit_holder(), resource_loader(), game_map(filename, unit_holder, render_stack, resource_loader), unit_manager(game_map, unit_holder) {

    // Creation of test playable
    # warning Move mesh loading into playable loading
    Mesh& playable_mesh_ref = resource_loader.loadMesh("airship.dae");
    Shader& playable_shader_ref = resource_loader.loadShader("shaders/doodad.vs",
        "shaders/doodad.fs");
    float playable_scale = 1.0f;

    for(int i = 0; i < 1; ++i){
        for(int j = 0; j < 1; ++j){
            glm::vec3 playable_position = glm::vec3(-10 - 3.0f*i, 0.0f, 5 - 3.0f*j);
            Playable temp(playable_mesh_ref, playable_shader_ref, playable_position, playable_scale);
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

ResourceLoader& Level::getResourceLoader() {
    return resource_loader;
}

UnitHolder& Level::getUnitHolder() {
    return unit_holder;
}

string Level::asJsonString() {
    string json_string = "{\n";

    json_string += resource_loader.asJsonString() + ",\n";
    json_string += game_map.asJsonString() + "\n";

    json_string += "}";

    return json_string;
}

void Level::saveAs(string filepath) {
    Debug::info("Saving level to %s\n", filepath.c_str());

    // Write level to file
    ofstream myfile;
    myfile.open(filepath);
    myfile << asJsonString();
    myfile.close();
}
