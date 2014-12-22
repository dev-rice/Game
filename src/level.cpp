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
    proj_matrix = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
    
    // camera = new Camera(glm::vec3(0.0f, 20.0f, 20.0f));
    // camera->setRotation(glm::vec3(0.78f, 0.0f, 0.0f));
    camera = new Camera(glm::vec3(0.0f, 2.0f, 4.0f));
    view_matrix = camera->getViewMatrix();
    
    GLuint doodad_vs = ShaderLoader::loadVertexShader("shaders/multiple_lights.vs");
    GLuint doodad_fs = ShaderLoader::loadFragmentShader("shaders/multiple_lights.fs");
    doodad_shader = ShaderLoader::combineShaderProgram(doodad_vs, doodad_fs);

    GLuint particle_vs = ShaderLoader::loadVertexShader("shaders/particle.vs");
    GLuint particle_fs = ShaderLoader::loadFragmentShader("shaders/particle.fs");
    GLuint particle_shader = ShaderLoader::combineShaderProgram(particle_vs, particle_fs);
   
    emitters.push_back(new FireEmitter(particle_shader, glm::vec3(-1.4f, 0.0f, 0.0f), 0.7f));
    // emitters.push_back(new SmokeEmitter(particle_shader, glm::vec3(-1.4f, 0.0f, 0.0f), 0.7f));

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
            
            drawable = NULL;
            mesh = NULL;
            
            delete drawable;
            delete mesh;
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

