#include "game_map.hpp"

GameMap::GameMap(std::string map_filename) : camera(), ground(), shadowbuffer() {

    ifstream map_input(map_filename);
    load(map_input);

    initializeGlobalUniforms();

}

void GameMap::render(){
    // Update the global uniforms like the camera position and shadow projections
    updateGlobalUniforms();

    // Draw all the drawables
    for (Doodad& doodad : doodads){
        doodad.draw();
    }

    // Draw all the particle emitters
    if (Profile::getInstance()->isParticlesOn()){
        for(Emitter& emitter : emitters){
            emitter.draw(&camera);
        }
    }

    // Draw the ground (terrain)
    ground.draw();

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
    for (const Json::Value& doodad_json : doodads_json){
        // NOOOOOOOO!!!!!! but for now...
        Mesh* mesh_ptr = new Mesh(doodad_json["mesh"].asString());

        glm::vec3 position;
        position.x = doodad_json["position"]["x"].asFloat();
        position.y = doodad_json["position"]["y"].asFloat();
        position.z = doodad_json["position"]["z"].asFloat();

        glm::vec3 rotation;
        position.x = doodad_json["rotation"]["x"].asFloat();
        position.y = doodad_json["rotation"]["y"].asFloat();
        position.z = doodad_json["rotation"]["z"].asFloat();

        Doodad doodad(mesh_ptr);
        doodad.setPosition(position);
        doodad.setRotationEuler(rotation);

        doodads.push_back(doodad);

    }
}

void GameMap::initializeGlobalUniforms(){
    // Create the uniform buffer object for the camera.
    glGenBuffers(1, &camera_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, camera_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 1, camera_ubo, 0, sizeof(glm::mat4) * 2);

    // Create the uniform buffer object for the shadows.
    glGenBuffers(1, &shadow_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, shadow_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, shadow_ubo, 0, sizeof(glm::mat4) * 2);

}

void GameMap::updateGlobalUniforms(){
    // Update the shadow view matrix based on the current
    // camera position
    glm::vec3 light_direction = glm::vec3(1.0f, 2.0f, -0.5f);
    glm::vec3 camera_offset = glm::vec3(camera.getPosition().x, 0, camera.getPosition().z - 40.0);

    // Ideally this shouldn't be created each time
    glm::mat4 depth_view = glm::lookAt(light_direction + camera_offset, camera_offset, glm::vec3(0,1,0));
    glm::mat4 depth_proj = glm::ortho<float>(-60,60,-65, 60,-40,40);

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 proj = camera.getProjectionMatrix();

    // Attach the shadow texture to location 4
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, shadowbuffer.getTexture());

    // Put the data in the UBO.
    glBindBuffer(GL_UNIFORM_BUFFER, camera_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
        glm::value_ptr(view));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
        glm::value_ptr(proj));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBuffer(GL_UNIFORM_BUFFER, shadow_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
        glm::value_ptr(depth_view));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
        glm::value_ptr(depth_proj));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

}
