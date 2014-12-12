// level_loader.cpp
// Trevor Westphal

#include "level.h"

Level::~Level(){
    #warning This leaks memory here, but correcting it causes a segfault
    // See http://stackoverflow.com/questions/13223399/c-deleting-a-pointer

    //emitter->~Emitter();
    //Manual deletion doesn't work either :(

    emitter = NULL;
    delete emitter;
}

Level::Level(GLFWwindow* window, const char* filename){
    this->window = window;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    proj_matrix = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
    
    camera = new Camera(glm::vec3(0.0f, 20.0f, 20.0f));
    camera->setRotation(glm::vec3(0.78f, 0.0f, 0.0f));
    view_matrix = camera->getViewMatrix();
    
    GLuint vertex_shader = ShaderLoader::loadVertexShader("shaders/thingy.vs");
    GLuint fragment_shader = ShaderLoader::loadFragmentShader("shaders/thingy.fs");
    shader_program = ShaderLoader::combineShaderProgram(vertex_shader, fragment_shader);

    emitter = new Emitter(shader_program);

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

    emitter->draw(camera, &proj_matrix);
    
}

void Level::loadLevel(const char* fileName){

    char buffer[128];
    char objectFileName[40];
    char textureFileName[40];
    int objectIndex, diffIndex, specIndex, normIndex, emitIndex;
    float x, y, z, scale, x_rot, y_rot, z_rot;

    FILE * ifile;
     ifile = fopen(fileName, "r");

    if(ifile == NULL){
        printf("Error opening file %s\n", fileName);
        return;
    
    }

     while(! feof(ifile)){
        if(fgets(buffer, 128, ifile) == NULL){
            // Can't read into buffer
            break;
        }

        if(buffer[0] == 'm'){
            sscanf(buffer, "%*c %s", objectFileName);
            // Add this mesh to meshes
            char actual[80] = "";
            strcat(actual, MODEL_PATH);
            strcat(actual, objectFileName);
            Mesh* mesh = new Mesh(actual);
            mesh->attachGeometryToShader(shader_program);
            meshes.push_back(mesh);

            mesh = NULL;
            delete mesh;
        }

        if(buffer[0] == 't'){
            sscanf(buffer, "%*c %s", textureFileName);
            
            char actual[80] = "";
            strcat(actual, TEXTURE_PATH);
            strcat(actual, textureFileName);

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

            Doodad* drawable = new Doodad(mesh, shader_program, position, rotation, scale);
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

void Level::handleInputs(){
    #warning Inputs should not be handled in the Level

    glfwPollEvents();
    
    // Camera controls
    // Movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera->moveZ(-1);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera->moveZ(1);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera->moveX(1);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera->moveX(-1);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        camera->moveY(-1);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        camera->moveY(1);
    }

    // Rotation
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        camera->rotateY(1);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        camera->rotateY(-1);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        camera->rotateX(1);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
        camera->rotateX(-1);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
        camera->rotateZ(1);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
        camera->rotateZ(-1);
    }
}