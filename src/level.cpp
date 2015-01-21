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

    particle_shader = ShaderLoader::loadShaderProgram("shaders/particle.vs",
        "shaders/particle.fs");

    shadow_shader = ShaderLoader::loadShaderProgram("shaders/shadow.vs",
        "shaders/shadow.fs");

    shadowbuffer = new Shadowbuffer(1.0);

    glm::vec3 light_direction = glm::vec3(-1.0f, 1.0f, 0.5f);
    depth_view = glm::lookAt(light_direction, glm::vec3(0,0,0),
        glm::vec3(0,1,0));
    // Size of the box to render (tailored to fit current map).
    // depth_proj = glm::ortho<float>(-50,50,-50, 50,-20,20);
    depth_proj = glm::ortho<float>(-10,10,-10, 10,-20,20);

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

    glBindBufferRange(GL_UNIFORM_BUFFER, 1, camera_ubo, 0, sizeof(glm::mat4) * 2);
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, shadow_ubo, 0, sizeof(glm::mat4) * 2);

    loadLevel(filename);

    // Creation of test playable
    Mesh *playable_mesh = new Mesh("res/models/demo_unit.obj");
    GLuint playable_shader = ShaderLoader::loadShaderProgram("shaders/doodad.vs",
        "shaders/doodad.fs");
    glm::vec3 playable_position = glm::vec3(0.0f, 0.0f, 0.0f);
    float playable_scale = 1.0f;

    units.push_back(new Playable(playable_mesh, playable_shader, playable_position, playable_scale));


}

void Level::draw(){
    // Update the view matrix based on the current
    // camera location / position
    updateGlobalUniforms();

    // Draw all the drawables
    for (int i = 0; i < drawables.size(); ++i){
        drawables[i]->draw();
    }

    for (int i = 0; i < units.size(); ++i){
        units[i]->update(ground);
        units[i]->draw();
    }

    // Draw all the particle emitters
    for(int i(0); i < emitters.size(); ++i){
        emitters[i]->draw(camera);
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

}

void Level::loadLevel(const char* filename){

    char buffer[128];
    char object_filename[40];
    char texture_filename[40];
    char particle_name[10];
    int objectIndex, diffIndex, specIndex, normIndex, emitIndex;
    float x, y, z, scale, x_rot, y_rot, z_rot;

    FILE * ifile;
    ifile = fopen(filename, "r");

    if(ifile == NULL){
        Debug::error("Error opening file %s\n", filename);
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
            sscanf(buffer, "%*c %d %d %d %d %d %f %f %f %f %f %f %f",
                &objectIndex, &diffIndex, &specIndex, &normIndex, &emitIndex,
                &x, &y, &z, &scale, &x_rot, &y_rot, &z_rot);

            Mesh* mesh = meshes[objectIndex];

            GLuint diffuse = getTexture(diffIndex);
            GLuint specular = getTexture(specIndex);
            GLuint emissive = getTexture(emitIndex);
            GLuint normal = getTexture(normIndex);

            glm::vec3 position = glm::vec3(x, y, z);
            glm::vec3 rotation = glm::vec3(x_rot, y_rot, z_rot);

            Doodad* drawable = new Doodad(mesh, doodad_shader, position,
                rotation, scale);
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
            char parameter[64];
            char ground_texture[64];
            sscanf(buffer, "%*c %s %s", parameter, ground_texture);

            char heightmap_filename[80] = "";
            strcat(heightmap_filename, TEXTURE_PATH);
            strcat(heightmap_filename, parameter);

            char ground_filename[80] = "";
            strcat(ground_filename, TEXTURE_PATH);
            strcat(ground_filename, ground_texture);

            std::string heightmap_filename_str(heightmap_filename);

            GLuint terrain_shader = ShaderLoader::loadShaderProgram(
                "shaders/terrain.vs", "shaders/terrain.fs");

            ground = new Terrain(terrain_shader, heightmap_filename_str);

            GLuint diffuse = TextureLoader::loadTextureFromFile(ground_filename,
                GL_LINEAR);
            GLuint normal = TextureLoader::loadTextureFromFile(
                "res/textures/rough_ground_norm.png", GL_LINEAR);

            ground->setDiffuse(diffuse);
            ground->setNormal(normal);

            drawables.push_back((Drawable*) ground);
        }

        if(buffer[0] == 'g'){
            // For now, a simple comment
            Debug::info("Found a ground texture set!\n");
        }

    }

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
