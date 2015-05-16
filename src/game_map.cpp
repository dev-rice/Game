#include "game_map.hpp"

GameMap::GameMap(std::string map_filename) : camera(), terrain() {
    ifstream map_input(map_filename);
    load(map_input);
}

void GameMap::render(){

}

void GameMap::load(ifstream& map_input){
    // Read the file into a string
    // http://bit.ly/1aM8TXS
    string map_contents((istreambuf_iterator<char>(map_input)), istreambuf_iterator<char>());

    // Use jsoncpp to parse the file contents
    Json::Value root;
    Json::Reader reader;
    bool parsing_successful = reader.parse(map_contents, root);
    if (!parsing_successful){
        Debug::error("Failed to parse map\n%s", reader.getFormattedErrorMessages().c_str());
    }

    // The filepaths for mesh and textures so we know where to load the files from
    string mesh_path = root["mesh_path"].asString();
    string texture_path = root["texture_path"].asString();

    // Read in the camera data
    const Json::Value camera_json = root["camera"];

    float fov = camera_json["fov"].asFloat();

    glm::vec3 position;
    position.x = camera_json["position"]["x"].asFloat();
    position.y = camera_json["position"]["y"].asFloat();
    position.z = camera_json["position"]["z"].asFloat();

    glm::vec3 rotation;
    rotation.x = camera_json["rotation"]["x"].asFloat();
    rotation.x = camera_json["rotation"]["y"].asFloat();
    rotation.x = camera_json["rotation"]["z"].asFloat();

    float move_sensitivity = camera_json["move_sensitivity"].asFloat();
    float rotate_sensitivity = camera_json["rotate_sensitivity"].asFloat();

    camera = Camera(position, rotation, move_sensitivity, rotate_sensitivity, fov);

    // Read in each doodad
    const Json::Value doodads_json = root["doodads"];
    for (const Json::Value& doodad : doodads_json){

    }
}
