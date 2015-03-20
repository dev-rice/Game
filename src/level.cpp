// level_loader.cpp
// Trevor Westphal

#include "level.h"

Level::~Level(){
    Debug::info("Calling the Level destructor.");
    #warning Deletion needs to be properly implemented
}

Level::Level(const char* filename){
    this->window = Window::getInstance();

    int width  = window->getWidth();
    int height = window->getHeight();

    proj_matrix = glm::perspective(45.0f, (float)width / (float)height, 0.1f,
        500.0f);

    camera = new Camera(glm::vec3(0.0f, 40.0f, 40.0f), glm::vec3(1.04f, 0.0f, 0.0f));
    // camera = new Camera(glm::vec3(7.39f, 2.91f, 1.13f), glm::vec3(0.07, 1.2, 0.0));

    doodad_shader = ShaderLoader::loadShaderProgram("shaders/doodad.vs",
        "shaders/doodad.fs");

    terrain_shader = ShaderLoader::loadShaderProgram("shaders/terrain.vs",
        "shaders/terrain.fs");

    particle_shader = ShaderLoader::loadShaderProgram("shaders/particle.vs",
        "shaders/particle.fs");

    shadow_shader = ShaderLoader::loadShaderProgram("shaders/shadow.vs",
        "shaders/shadow.fs");

    shadowbuffer = new Shadowbuffer(1.0);

    glm::vec3 light_direction = glm::vec3(1.0f, 2.0f, -0.5f);
    depth_view = glm::lookAt(light_direction, glm::vec3(0,0,0),
        glm::vec3(0,1,0));
    // Size of the box to render (tailored to fit current map).
    depth_proj = glm::ortho<float>(-60,60,-65, 60,-40,40);

    // Create the uniform buffer object.
    glGenBuffers(1, &camera_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, camera_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Create the uniform buffer object.
    glGenBuffers(1, &shadow_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, shadow_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Create the uniform buffer object.
    glGenBuffers(1, &unit_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, unit_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 1, camera_ubo, 0, sizeof(glm::mat4) * 2);
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, shadow_ubo, 0, sizeof(glm::mat4) * 2);
    glBindBufferRange(GL_UNIFORM_BUFFER, 10, unit_ubo, 0, sizeof(glm::vec3));

    loadLevel(filename);

    PathFinder::allocateArray(ground);

    // Creation of test playable
    Mesh* playable_mesh = new Mesh("res/models/human.dae");
    GLuint playable_shader = ShaderLoader::loadShaderProgram("shaders/doodad.vs",
        "shaders/doodad.fs");
    float playable_scale = 1.0f;

    for(int i = 0; i < 1; ++i){
        for(int j = 0; j < 1; ++j){
            glm::vec3 playable_position = glm::vec3(3.0f*i, 0.0f, 3.0f*j);
            Playable* temp = new Playable(playable_mesh, playable_shader, playable_position, playable_scale);
            temp->loadFromXML("res/units/testunit.xml");

            if (rand() % 2){
                temp->setDiffuse(TextureLoader::loadBlue());
            } else {
                temp->setDiffuse(TextureLoader::loadGreen());
            }

            drawables.push_back(temp);
            units.push_back(temp);
        }
    }

    Mesh* water_plane = new PlaneMesh();
    Doodad* water_doodad = new Doodad(water_plane);

    GLuint water_texture = TextureLoader::loadTextureFromPixel(0.1f, 0.72f, 0.95f, 0.6f);
    water_doodad->setEmissive(water_texture);

    water_doodad->setPosition(-30.5, 8.5, -49.0);
    water_doodad->setScale(16.0);
    water_doodad->rotateGlobalEuler(M_PI/2.0f, 0.0f, 0.0f);

    non_shadow_drawables.push_back(water_doodad);

}

void Level::draw(){
    updateGlobalUniforms();

    // make sure the selected units are good to go
    std::vector<Playable*> new_selected_units;
    for(int i = 0; i < selected_units.size(); ++i){
        if(selected_units[i]->isSelected()){
            new_selected_units.push_back(selected_units[i]);
        }
    }
    selected_units.clear();
    selected_units = new_selected_units;

    // update all the units
    for (int i = 0; i < units.size(); ++i){
       units[i]->update(ground, &units);
    }

    // Draw all the drawables
    for (int i = 0; i < drawables.size(); ++i){
        drawables[i]->draw();
    }

    // Draw all the drawables that don't need shadows.
    for (int i = 0; i < non_shadow_drawables.size(); ++i){
        non_shadow_drawables[i]->draw();
    }

    // Draw all the particle emitters
    if (Profile::getInstance()->isParticlesOn()){
        for(int i(0); i < emitters.size(); ++i){
            emitters[i]->draw(camera);
        }
    }

}

void Level::drawShadowMap(){
    updateGlobalUniforms();

    for (int i = 0; i < drawables.size(); ++i){
        // Save the shader this drawable is currently using
        GLuint current_shader = drawables[i]->getShader();
        // Set the drawable to render with the shadow shader
        drawables[i]->setShader(shadow_shader);
        // Draw the drawable from the light's perspective
        drawables[i]->draw();
        // Reset the drawable's shader to what it was before
        drawables[i]->setShader(current_shader);
    }
}

void Level::updateGlobalUniforms(){
    // Update the shadow view matrix based on the current
    // camera position
    glm::vec3 light_direction = glm::vec3(1.0f, 2.0f, -0.5f);
    glm::vec3 camera_offset = glm::vec3(camera->getPosition().x, 0, camera->getPosition().z - 40.0);
    depth_view = glm::lookAt(light_direction + camera_offset, camera_offset,
        glm::vec3(0,1,0));

    view_matrix = camera->getViewMatrix();

    // Attach the shadow texture to location 4
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, shadowbuffer->getTexture());

    // Put the data in the UBO.
    glBindBuffer(GL_UNIFORM_BUFFER, camera_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
        glm::value_ptr(view_matrix));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
        glm::value_ptr(proj_matrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBuffer(GL_UNIFORM_BUFFER, shadow_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
        glm::value_ptr(depth_view));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
        glm::value_ptr(depth_proj));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glm::vec3 unit_pos = units[0]->getPosition();
    glBindBuffer(GL_UNIFORM_BUFFER, unit_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3),
        glm::value_ptr(unit_pos));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);



}

glm::vec3 Level::getIntersection(glm::vec3 line, float height){
    glm::vec3 line_start = camera->getPosition();
    glm::vec3 normal = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 plane = glm::vec3(0.0, height, 0.0);

    // To find the point on the plane of clicking (defined by mouse_plane)
    // From http://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
    float d = glm::dot((plane - line_start), normal) / glm::dot(line, normal);
    glm::vec3 intersection = d * line + line_start;

    return intersection;
}

glm::vec3 Level::calculateRay(glm::vec2 screen_point){
    glm::mat4 view = camera->getViewMatrix();
    glm::vec3 ray = glm::vec3(glm::inverse(proj_matrix) * glm::vec4(screen_point, -1.0, 1.0));
    ray.z = -1.0;
    ray = glm::vec3(glm::inverse(view) * glm::vec4(ray, 0.0));
    ray = glm::normalize(ray);
    return ray;
}

std::tuple<float, float, glm::vec3> Level::findWorldPoint(glm::vec3 ray, int steps, float bottom, float top){
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

        float terrain_height = ground->getHeightInterpolated(world_point.x, world_point.z);

        if (height > terrain_height){
            top_bound = height;
        } else if (height < terrain_height){
            bottom_bound = height;
            break;
        }
    }

    return std::make_tuple(bottom_bound, top_bound, world_point);
}

glm::vec3 Level::findWorldPointInit(glm::vec3 ray, int steps){
    // Ideal mouse point search algorithm
    // Do a low resolution pass of the planes and find
    // which planes the point is between. Then repeat
    // the process but this time only between those two
    // planes. Do this over and over until the mouse point
    // is valid.

    float top = ground->getMaxHeight() + 1.0;
    float bottom = 0;
    glm::vec3 world_point;

    std::tuple<float,float, glm::vec3> bounds;
    for (int i = 0; i < 10; ++i){
        bounds = findWorldPoint(ray, steps, bottom, top);
        bottom = std::get<0>(bounds);
        top = std::get<1>(bounds);
        world_point = std::get<2>(bounds);
    }

    return world_point;

}

glm::vec3 Level::calculateWorldPosition(glm::vec2 screen_point){
    glm::vec3 ray = calculateRay(screen_point);
    glm::vec3 world_point = findWorldPointInit(ray, 100);

    return world_point;
}

void Level::loadLevel(const char* filename){

    char buffer[128];
    char object_filename[40];
    char texture_filename[40];
    char particle_name[10];
    int objectIndex, diffIndex, specIndex, normIndex, emitIndex;
    float x, y, z, scale, x_rot, y_rot, z_rot;

    FILE* ifile;
    ifile = fopen(filename, "r");

    if(!ifile){
        Debug::error("Error opening file %s\n", filename);
        Debug::error("Falling back to default map '%s'.\n", DEFAULT_MAP);
        loadLevel(DEFAULT_MAP);
        return;
    }

     while(! feof(ifile)){
        if(fgets(buffer, 128, ifile) == NULL){
            // Can't read into buffer
            break;
        }

        if(buffer[0] == 'v'){
            sscanf(buffer, "%*c %f %f", &x, &z);
            camera->setPosition(camera->getPosition() + glm::vec3(x, 0.0, z));
        }

        if(buffer[0] == 'm'){
            sscanf(buffer, "%*c %s", object_filename);
            // Add this mesh to meshes
            char actual[80] = "";
            strcat(actual, MODEL_PATH);
            strcat(actual, object_filename);
            Mesh* mesh = new Mesh(actual);
            meshes.push_back(mesh);

            mesh = NULL;
            delete mesh;
        }

        if(buffer[0] == 't'){
            sscanf(buffer, "%*c %s", texture_filename);

            char actual[80] = "";
            strcat(actual, TEXTURE_PATH);
            strcat(actual, texture_filename);

            //  Add this texture to the texture container
            GLuint texture = TextureLoader::loadTextureFromFile(actual,
                GL_LINEAR);
            textures.push_back(texture);

        }

        if(buffer[0] == 'd'){
            float x_axis, y_axis, z_axis, angle;
            sscanf(buffer, "%*c %d %d %d %d %d %f %f %f %f %f %f %f %f",
                &objectIndex, &diffIndex, &specIndex, &normIndex, &emitIndex,
                &x, &y, &z, &scale, &x_axis, &y_axis, &z_axis, &angle);

            Mesh* mesh = meshes[objectIndex];

            GLuint diffuse = getTexture(diffIndex);
            GLuint specular = getTexture(specIndex);
            GLuint emissive = getTexture(emitIndex);
            GLuint normal = getTexture(normIndex);

            glm::vec3 position = glm::vec3(x, y, z);
            glm::vec3 rotation = glm::vec3(x_axis, y_axis, z_axis);

            Doodad* drawable = new Doodad(mesh, doodad_shader, position, scale);
            drawable->rotateAxisAngle(rotation, angle);
            drawable->setDiffuse(diffuse);
            drawable->setSpecular(specular);
            drawable->setEmissive(emissive);
            drawable->setNormal(normal);

            drawables.push_back(drawable);

        }

        if(buffer[0] == 'p'){
            sscanf(buffer, "%*c %s %f %f %f", particle_name, &x, &y, &z);
            Emitter *e = 0;
            if(strcmp(particle_name, "fire") == 0){
                e = new FireEmitter(particle_shader, glm::vec3(x, y, z), 0.6f);
            } else if (strcmp(particle_name, "snow") == 0){
                e = new SnowEmitter(particle_shader, glm::vec3(x, y, z));
            } else if (strcmp(particle_name, "smoke") == 0){
                e = new SmokeEmitter(particle_shader, glm::vec3(x, y, z), 0.7f);
            }

            if(e){
                emitters.push_back(e);
            } else {
                Debug::error("Could not load particle type: %s\n",
                    particle_name);
            }
        }

        if(buffer[0] == 'h'){
            char heightmap_name[64];
            float amplification;
            sscanf(buffer, "%*c %s %f", heightmap_name, &amplification);

            char heightmap_filename[80] = "";
            strcat(heightmap_filename, TEXTURE_PATH);
            strcat(heightmap_filename, heightmap_name);

            std::string heightmap_filename_str(heightmap_filename);

            ground = new Terrain(terrain_shader, heightmap_filename_str, amplification);

            drawables.push_back((Drawable*) ground);
        }

        if(buffer[0] == 'g'){
            // For now, a simple comment
            int texture_number;
            int splatmap_number;
            char diffuse_name[64];
            char splatmap_channel;
            sscanf(buffer, "%*c %d %d %c %s", &texture_number, &splatmap_number, &splatmap_channel, diffuse_name);

            char diffuse_filename[80] = "";
            strcat(diffuse_filename, TEXTURE_PATH);
            strcat(diffuse_filename, diffuse_name);

            GLuint diffuse = TextureLoader::loadTextureFromFile(diffuse_filename, GL_LINEAR);

            if (texture_number == 0){
                ground->addDiffuse(diffuse, 0, 0, 'r');

            } else {
                ground->addDiffuse(diffuse, splatmap_number, texture_number, splatmap_channel);
            }

        }

        if (buffer[0] == 's'){
            char splatmap_name[64];
            sscanf(buffer, "%*c %s", splatmap_name);

            char splatmap_filename[80] = "";
            strcat(splatmap_filename, TEXTURE_PATH);
            strcat(splatmap_filename, splatmap_name);

            GLuint splatmap = TextureLoader::loadTextureFromFile(splatmap_filename, GL_LINEAR);
            ground->addSplatmap(splatmap);
        }

    }

    ground->fillSplatmaps();

    fclose(ifile);
}

GLuint Level::getTexture(GLuint index){
    GLuint texture;
    if (index == 0){
        texture = 0;
    } else {
        texture = textures[index - 1];
    }

    return texture;
}

int Level::getMapDepth(){
    return ground->getDepth();
}

int Level::getMapWidth(){
    return ground->getWidth();
}

void Level::issueOrder(Playable::Order order, glm::vec3 target, bool should_enqueue){

    // even shorter circuit for "not my unit, can't command it".

    // short circuit for stop and hold position
    // No need to path!

    // We need to decide if it's targeting a unit with this command or not
    Playable* targeted_unit = 0;

    // Check all the other playables to see if one is the target
    for(int i = 0; i < units.size(); ++i){
        glm::vec3 unit_pos = units[i]->getPosition();
        float click_distance_from_unit = getDistance(unit_pos.x, unit_pos.z, target.x, target.z);

        if(click_distance_from_unit < units[i]->getRadius()){
            targeted_unit = units[i];
        }
    }
    // Debug::info("Selected units size: %d\n", selected_units.size());

    // If it's only one unit
    float x_center, z_center, smallest_radius;
    if (selected_units.size() != 0){
        x_center = selected_units[0]->getPosition().x;
        z_center = selected_units[0]->getPosition().z;
        smallest_radius = selected_units[0]->getRadius();
    } else {
        // Short circuit for bug when the selected units is empty
        return;
    }

    // Make sure if it skips the click distance calculation so that it will not offset
    float click_distance = -1.0f;
    float max_distance = 0.0f;

    // If there is more than one unit, setup the magic box
    if(selected_units.size() > 1){
        float x_sum = 0.0f;
        float z_sum = 0.0f;

        for(int i = 0; i < selected_units.size(); ++i){
            glm::vec3 unit_pos = selected_units[i]->getPosition();
            x_sum += unit_pos.x;
            z_sum += unit_pos.z;
        }

        x_center = x_sum / selected_units.size();
        z_center = z_sum / selected_units.size();

        for(int i = 0; i < selected_units.size(); ++i){

            glm::vec3 unit_pos = selected_units[i]->getPosition();
            float distance = getDistance(unit_pos.x, unit_pos.z, x_center, z_center);

            if(distance > max_distance){
                max_distance = distance;
            }

            if(selected_units[i]->getRadius() < smallest_radius){
                smallest_radius = selected_units[i]->getRadius();
            }
        }

        click_distance = getDistance(target.x, target.z, x_center, z_center);
    }

    // Start logging the pathfinding time
    float start_time = GameClock::getInstance()->getCurrentTime();

    // Create the path for all units in the selection
    std::vector<glm::vec3> path = PathFinder::find_path(ground,
                                                        int(x_center),
                                                        int(z_center),
                                                        int(target.x),
                                                        int(target.z),
                                                        smallest_radius);

    // End logging and report
    float delta_time = GameClock::getInstance()->getCurrentTime() - start_time;
    Debug::info("Took %.2f seconds to find the path.\n", delta_time);

    // Issue the appropriate order
    for(int i = 0; i < selected_units.size(); ++i){

        float x_to_move = target.x;
        float z_to_move = target.z;
        glm::vec3 unit_pos = selected_units[i]->getPosition();

        if(click_distance > max_distance){
            x_to_move += (unit_pos.x - x_center);
            z_to_move += (unit_pos.z - z_center);
        }

        selected_units[i]->receiveOrder(order, glm::vec3(x_to_move, 0.0f, z_to_move), should_enqueue, path, targeted_unit);
    }
}

float Level::getDistance(float a1, float a2, float b1, float b2){
    float x_diff = abs(a1 - b1);
    float z_diff = abs(a2 - b2);
    return sqrt(x_diff*x_diff + z_diff*z_diff);
}


void Level::selectUnit(glm::vec3 click){

    std::vector<Playable*> selected_units_copy = selected_units;
    selected_units.clear();

    float nearest = FLT_MAX;
    Playable* nearest_playable = 0;

    for(int i = 0; i < units.size(); ++i){

        glm::vec3 unit_pos = units[i]->getPosition();
        float distance = getDistance(unit_pos.x, unit_pos.z, click.x, click.z);

        if( distance < units[i]->getRadius() && distance < nearest){
            nearest = distance;
            nearest_playable = units[i];
        } else {
            units[i]->deSelect();
        }
    }

    // If we found one that was clicked on and is the nearest
    if(nearest_playable){
        nearest_playable->select();
        selected_units.push_back(nearest_playable);
    } else {
        selected_units = selected_units_copy;

        for(int i = 0; i < selected_units.size(); ++i){
            selected_units[i]->select();
        }
    }
}

void Level::selectUnits(glm::vec3 coord_a, glm::vec3 coord_b){

    std::vector<Playable*> selected_units_copy = selected_units;
    selected_units.clear();

    float left = std::min(coord_a.x, coord_b.x);
    float right = std::max(coord_a.x, coord_b.x);

    float down = std::min(coord_a.z, coord_b.z);
    float up = std::max(coord_a.z, coord_b.z);

    for(int i = 0; i < units.size(); ++i){
        if(units[i]->isTempSelected()){
            units[i]->select();
            units[i]->tempDeSelect();
            selected_units.push_back(units[i]);
        } else {
            units[i]->deSelect();
        }
    }

    if(selected_units.size() == 0){
        selected_units = selected_units_copy;

        for(int i = 0; i < selected_units.size(); ++i){
            selected_units[i]->select();
        }
    }
}

void Level::tempSelectUnits(glm::vec3 coord_a, glm::vec3 coord_b){
    float left = std::min(coord_a.x, coord_b.x);
    float right = std::max(coord_a.x, coord_b.x);

    float down = std::min(coord_a.z, coord_b.z);
    float up = std::max(coord_a.z, coord_b.z);

    for(int i = 0; i < units.size(); ++i){
        glm::vec3 unit_pos = units[i]->getPosition();
        float radius = units[i]->getRadius();

        if(left - radius < unit_pos.x && right + radius > unit_pos.x && down - radius < unit_pos.z && up + radius > unit_pos.z){
            units[i]->tempSelect();
        } else {
            units[i]->tempDeSelect();
        }
    }
}
