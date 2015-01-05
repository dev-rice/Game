// level_loader.cpp
// Trevor Westphal

#include "level.h"

Level::~Level(){
    printf("Calling the Level destructor.");
    #warning Deletion needs to be properly implemented
}

Level::Level(Window* window, const char* filename){
    this->window = window;

    int width  = window->getWidth();
    int height = window->getHeight();

    proj_matrix = glm::perspective(45.0f, (float)width / (float)height, 0.1f,
        500.0f);

    camera = new Camera(glm::vec3(0.0f, 40.0f, 40.0f));
    camera->setRotation(glm::vec3(1.04f, 0.0f, 0.0f));
    // camera = new Camera(glm::vec3(0.0f, 2.0f, 4.0f));

    doodad_shader = ShaderLoader::loadShaderProgram("shaders/multiple_lights.vs",
        "shaders/multiple_lights.fs");

    particle_shader = ShaderLoader::loadShaderProgram("shaders/particle.vs",
        "shaders/particle.fs");

    shadow_shader = ShaderLoader::loadShaderProgram("shaders/shadow.vs",
        "shaders/shadow.fs");

    glm::vec3 light_direction = glm::vec3(-1.0f, 1.0f, 0.0f);
    depth_view = glm::lookAt(light_direction, glm::vec3(0,0,0),
        glm::vec3(0,1,0));
    // Size of the box to render (tailored to fit current map).
    depth_proj = glm::ortho<float>(-50,50,-50, 50,-20,20);

    loadLevel(filename);
}

void Level::draw(){
    // Update the view matrix based on the current
    // camera location / position
    view_matrix = camera->getViewMatrix();

    // Draw all the drawables
    for (int i = 0; i < drawables.size(); ++i){
        // This is how you move things
        // glm::vec3 position = drawables[i].getPosition();
        // drawables[i].setPosition(position + glm::vec3(-0.01f, 0.0f, 0.0f));

        drawables[i]->draw(camera, &proj_matrix);
    }

    for(int i(0); i < emitters.size(); ++i){
        emitters[i]->draw(camera, &proj_matrix);
    }

}

void Level::drawShadowMap(){
    for (int i = 0; i < drawables.size(); ++i){
        drawables[i]->shadow_map = shadow_map;

        // Save the shader this drawable is currently using
        GLuint current_shader = drawables[i]->getShader();
        // Set the drawable to render with the shadow shader
        drawables[i]->setShader(shadow_shader);
        // Draw the drawable from the light's perspective
        drawables[i]->draw(&depth_view, &depth_proj);
        // Reset the drawable's shader to what it was before
        drawables[i]->setShader(current_shader);
    }
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
            GLuint normal = getTexture(normIndex);
            GLuint emissive = getTexture(emitIndex);

            texture_set = new TextureSet(diffuse, specular, normal, emissive);

            glm::vec3 position = glm::vec3(x, y, z);
            glm::vec3 rotation = glm::vec3(x_rot, y_rot, z_rot);

            Doodad* drawable = new Doodad(mesh, doodad_shader, position,
                rotation, scale);
            drawable->attachTextureSet(texture_set);
            drawable->depth_view = depth_view;
            drawable->depth_proj = depth_proj;

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
            sscanf(buffer, "%*c %s", parameter);

            char heightmap_filename[80] = "";
            strcat(heightmap_filename, TEXTURE_PATH);
            strcat(heightmap_filename, parameter);

            std::string heightmap_filename_str(heightmap_filename);
            ground = new Terrain(doodad_shader,
                heightmap_filename_str);

            GLuint diffuse = TextureLoader::loadTextureFromFile(
                "res/textures/ice_diff.png", GL_LINEAR);

            texture_set = new TextureSet(diffuse, 0, 0, 0);
            ground->attachTextureSet(texture_set);

            drawables.push_back((Drawable*) ground);
        }

        if(buffer[0] == 'g'){
            // For now, a simple comment
            Debug::info("Found a ground texture set!\n");
        }

        if(buffer[0] == 'c'){
            sscanf(buffer, "%*c %d %d %d %d",  &diffIndex, &specIndex, &normIndex, &emitIndex);

            GLuint diffuse = getTexture(diffIndex);
            GLuint specular = getTexture(specIndex);
            GLuint normal = getTexture(normIndex);
            GLuint emissive = getTexture(emitIndex);

            texture_set->load(diffuse, specular, normal, emissive);
            texture_set->makeCyclic(60);
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

int Level::getMapHeight(){
    return ground->getHeight();
}

int Level::getMapWidth(){
    return ground->getWidth();
}
