#include "game_map.hpp"

GameMap::GameMap(string map_filename) : camera(), ground(), shadowbuffer(1.0) {
    ifstream map_input(map_filename);
    load(map_input);

    shadow_shader = ShaderLoader::loadShaderProgram("shaders/shadow.vs", "shaders/shadow.fs");

    initializeGlobalUniforms();

}

void GameMap::render(){
    // Render the shadow map into the shadow buffer
    if (Profile::getInstance()->isShadowsOn()){
        renderToShadowMap();
    }

    // Update the global uniforms like the camera position and shadow projections
    updateGlobalUniforms();

    // Draw all the doodads
    for (Doodad& doodad : doodads){
        doodad.draw();
    }

    ground.draw();

    if (Profile::getInstance()->isParticlesOn()){
        for (Emitter* emitter : emitters){
            emitter->draw(&camera);
        }
    }

}

void GameMap::renderToShadowMap(){
    updateGlobalUniforms();

    RenderStack::getInstance()->pushFramebuffer(&shadowbuffer);

    for (Doodad& doodad : doodads){
        // Save the shader this drawable is currently using
        GLuint current_shader = doodad.getShader();
        // Set the drawable to render with the shadow shader
        doodad.setShader(shadow_shader);
        // Draw the drawable from the light's perspective
        doodad.draw();
        // Reset the drawable's shader to what it was before
        doodad.setShader(current_shader);
    }
    GLuint current_shader = ground.getShader();
    ground.setShader(shadow_shader);
    ground.draw();
    ground.setShader(current_shader);


    RenderStack::getInstance()->popFramebuffer();

}

glm::vec3 GameMap::calculateWorldPosition(glm::vec2 screen_point){
    glm::vec3 ray = calculateRay(screen_point);
    glm::vec3 world_point = findMapPointInit(ray, 100);

    return world_point;
}

Camera& GameMap::getCamera(){
    return camera;
}

Terrain& GameMap::getGround(){
    return ground;
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

    // Create the camera from the json segment
    camera = Camera(root["camera"]);

    // Read in each doodad
    const Json::Value doodads_json = root["doodads"];
    for (const Json::Value& doodad_json : doodads_json){
        // Create doodad from the json segment
        doodads.push_back(Doodad(doodad_json, mesh_path, texture_path));
    }

    // Create each particle emitter
    EmitterFactory emitter_factory;
    const Json::Value emitters_json = root["particle_emitters"];
    for (const Json::Value& emitter_json : emitters_json){
        emitters.push_back(emitter_factory.build(emitter_json));
    }

    // Create the ground from the json segment
    ground = Terrain(root["terrain"], texture_path);

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

glm::vec3 GameMap::getIntersection(glm::vec3 line, float height){
    glm::vec3 line_start = camera.getPosition();
    glm::vec3 normal = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 plane = glm::vec3(0.0, height, 0.0);

    // To find the point on the plane of clicking (defined by mouse_plane)
    // From http://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
    float d = glm::dot((plane - line_start), normal) / glm::dot(line, normal);
    glm::vec3 intersection = d * line + line_start;

    return intersection;
}

glm::vec3 GameMap::calculateRay(glm::vec2 screen_point){
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 proj = camera.getProjectionMatrix();

    glm::vec3 ray = glm::vec3(glm::inverse(proj) * glm::vec4(screen_point, -1.0, 1.0));
    ray.z = -1.0;
    ray = glm::vec3(glm::inverse(view) * glm::vec4(ray, 0.0));
    ray = glm::normalize(ray);
    return ray;
}

std::tuple<float, float, glm::vec3> GameMap::findMapPoint(glm::vec3 ray, int steps, float bottom, float top){
    // Search idea from http://bit.ly/1Jyb6pa
    glm::vec3 world_point;

    float height = top;
    float increment = (top - bottom) / (float)steps;

    float bottom_bound = bottom;
    float top_bound = top;
    for (int i = 0; i <= steps; ++i){
        top_bound = height;
        height = top - i * increment;
        world_point = getIntersection(ray, height);

        float terrain_height = ground.getHeightInterpolated(world_point.x, world_point.z);

        if (height > terrain_height){
            top_bound = height;
        } else if (height < terrain_height){
            bottom_bound = height;
            break;
        }
    }

    return std::make_tuple(bottom_bound, top_bound, world_point);
}

glm::vec3 GameMap::findMapPointInit(glm::vec3 ray, int steps){
    // Ideal mouse point search algorithm
    // Do a low resolution pass of the planes and find
    // which planes the point is between. Then repeat
    // the process but this time only between those two
    // planes. Do this over and over until the mouse point
    // is valid.

    float top = ground.getMaxHeight() + 1.0;
    float bottom = 0;
    glm::vec3 world_point;

    std::tuple<float,float, glm::vec3> bounds;
    for (int i = 0; i < 10; ++i){
        bounds = findMapPoint(ray, steps, bottom, top);
        bottom = std::get<0>(bounds);
        top = std::get<1>(bounds);
        world_point = std::get<2>(bounds);
    }

    return world_point;

}
