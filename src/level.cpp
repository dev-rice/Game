// level_loader.cpp
// Trevor Westphal

#include "level.h"

Level::~Level(){
    printf("Calling the Level destructor.");
    #warning Deletion needs to be properly implemented
}

Level::Level(GLFWwindow* window, const char* filename){
    this->window = window;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    proj_matrix = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 500.0f);

    // camera = new Camera(glm::vec3(0.0f, 20.0f, 20.0f));
    // camera->setRotation(glm::vec3(0.78f, 0.0f, 0.0f));
    camera = new Camera(glm::vec3(0.0f, 2.0f, 4.0f));
    view_matrix = camera->getViewMatrix();

    GLuint doodad_vs = ShaderLoader::loadVertexShader("shaders/doodad.vs");
    GLuint doodad_fs = ShaderLoader::loadFragmentShader("shaders/doodad.fs");
    doodad_shader = ShaderLoader::combineShaderProgram(doodad_vs, doodad_fs);

    GLuint particle_vs = ShaderLoader::loadVertexShader("shaders/particle.vs");
    GLuint particle_fs = ShaderLoader::loadFragmentShader("shaders/particle.fs");
    particle_shader = ShaderLoader::combineShaderProgram(particle_vs, particle_fs);

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
        printf("Error opening file %s\n", filename);
        return;

    }

     while(! feof(ifile)){
        if(fgets(buffer, 128, ifile) == NULL){
            // Can't read into buffer
            break;
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
            GLuint texture = TextureLoader::loadTextureFromFile(actual, GL_LINEAR);
            textures.push_back(texture);

        }

        if(buffer[0] == 'd'){
            sscanf(buffer, "%*c %d %d %d %d %d %f %f %f %f %f %f %f", &objectIndex, &diffIndex, &specIndex, &normIndex, &emitIndex, &x, &y, &z, &scale, &x_rot, &y_rot, &z_rot);

            Mesh* mesh = meshes[objectIndex];

            GLuint diffuse = getTexture(diffIndex);
            GLuint specular = getTexture(specIndex);
            GLuint normal = getTexture(normIndex);
            GLuint emissive = getTexture(emitIndex);

            TextureSet texture_set(diffuse, specular, normal, emissive);

            glm::vec3 position = glm::vec3(x, y, z);
            glm::vec3 rotation = glm::vec3(x_rot, y_rot, z_rot);

            Doodad* drawable = new Doodad(mesh, doodad_shader, position, rotation, scale);
            drawable->attachTextureSet(texture_set);
            drawables.push_back(drawable);

        }

        if(buffer[0] == 'p'){
            sscanf(buffer, "%*c %s %f %f %f", particle_name, &x, &y, &z);
            Emitter *e = 0;
            if(strcmp(particle_name, "fire") == 0){
                e = new FireEmitter(particle_shader, glm::vec3(x, y, z), 0.7f);
            } else if (strcmp(particle_name, "snow") == 0){
                e = new SnowEmitter(particle_shader, glm::vec3(x, y, z));
            } else if (strcmp(particle_name, "smoke") == 0){
                e = new SmokeEmitter(particle_shader, glm::vec3(x, y, z), 0.7f);
            }

            if(e){
                emitters.push_back(e);
            } else {
                Debug::error("Could not load particle type: %s\n", particle_name);
            }
        }

        if(buffer[0] == 'h'){
            Debug::info("Found a heightmap!\n");
            char parameter[64];
            sscanf(buffer, "%*c %s", parameter);

            char heightmap_filename[80] = "";
            strcat(heightmap_filename, TEXTURE_PATH);
            strcat(heightmap_filename, parameter);

            std::string heightmap_filename_str(heightmap_filename);
            Drawable* ground = new Terrain(doodad_shader, heightmap_filename_str);

            GLuint diffuse = TextureLoader::loadTextureFromFile("res/textures/rough_ground.png", GL_LINEAR);
            GLuint heightmap = TextureLoader::loadTextureFromFile(heightmap_filename, GL_LINEAR);

            TextureSet texture_set(diffuse, 0, 0, 0);
            ground->attachTextureSet(texture_set);

            drawables.push_back(ground);
        }

        if(buffer[0] == 'g'){
            Debug::info("Found a ground texture set!\n");
        }
    }



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
