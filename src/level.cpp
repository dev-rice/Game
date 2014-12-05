// level_loader.cpp
// Trevor Westphal

#include "level.h"

Level::Level(GLFWwindow* window){
    this->window = window;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    proj_matrix = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
    
    camera = Camera(glm::vec3(0.0f, 0.0f, 10.0f));
    view_matrix = camera.getViewMatrix();
    
    light = Light(glm::vec3(3.0f, 2.5f, 2.0f), glm::vec3(1.0f, 0.9f, 0.9f), 10.0f);

    GLuint vertex_shader = ShaderLoader::loadVertexShader("shaders/vertex_shader.glsl");
    GLuint fragment_shader = ShaderLoader::loadFragmentShader("shaders/fragment_shader.glsl");
    shader_program = ShaderLoader::combineShaderProgram(vertex_shader, fragment_shader);
}

void Level::draw(){
    // Update the view matrix based on the current
    // camera location / position
    view_matrix = camera.getViewMatrix();
    
    // Draw all the drawables
    for (int i = 0; i < drawables.size(); ++i){
        // This is how you move things
        // glm::vec3 position = drawables[i].getPosition();
        // drawables[i].moveTo(position + glm::vec3(-0.01f, 0.0f, 0.0f));
        
        drawables[i].draw(&view_matrix, &proj_matrix, &light);
    }
}

void Level::loadLevel(const char * fileName){

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
            printf("Mesh: %s\n", objectFileName);
            // Add this mesh to meshes
            char actual[80] = "";
            strcat(actual, MODEL_PATH);
            strcat(actual, objectFileName);
            Mesh mesh = Mesh(actual, 1.0f);
            mesh.attachShader(shader_program);
            meshes.push_back(mesh);
        }

        if(buffer[0] == 't'){
            sscanf(buffer, "%*c %s", textureFileName);
            printf("Texture: %s\n", textureFileName);
            
            char actual[80] = "";
            strcat(actual, TEXTURE_PATH);
            strcat(actual, textureFileName);

            //  Add this texture to the texture container
            textures.addTexture(textureFileName, GL_LINEAR);

        }

        if(buffer[0] == 'd'){
            sscanf(buffer, "%*c %d %d %d %d %d %f %f %f %f %f %f %f", &objectIndex, &diffIndex, &specIndex, &normIndex, &emitIndex, &x, &y, &z, &scale, &x_rot, &y_rot, &z_rot);

            Mesh* mesh = &meshes[objectIndex];

            printf("Loading object with texture indeces:\n");
            printf("    diffuse = %d\n", diffIndex);
            printf("    specular = %d\n", specIndex);
            printf("    normal = %d\n", normIndex);
            printf("    emissive = %d\n", emitIndex);

            GLint diffuse = textures.getTexture(diffIndex);
            GLint specular = textures.getTexture(specIndex);
            GLint normal = textures.getTexture(normIndex);
            GLint emissive = textures.getTexture(emitIndex);

            printf("    diffuse = %d\n", diffuse);
            printf("    specular = %d\n", specular);
            printf("    normal = %d\n", normal);
            printf("    emissive = %d\n", emissive);

            TextureSet texture_set;
            texture_set.diffuse = diffuse;
            texture_set.specular = specular;
            texture_set.normal = normal;
            texture_set.emissive = emissive;

            glm::vec3 position = glm::vec3(x, y, z);
            glm::vec3 rotation = glm::vec3(x_rot, y_rot, z_rot);

            Drawable drawable = Drawable(mesh, position, rotation);
            drawable.attachTextureSet(texture_set);
            drawables.push_back(drawable);

            mesh = NULL;
            delete mesh;
        }
    }

}

void Level::handleInputs(){
    glfwPollEvents();
    
    // Camera controls
    // Movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.moveZ(-1);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.moveZ(1);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.moveX(1);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.moveX(-1);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        camera.moveY(-1);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        camera.moveY(1);
    }

    // Rotation
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        camera.rotateY(1);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        camera.rotateY(-1);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        camera.rotateX(1);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
        camera.rotateX(-1);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
        camera.rotateZ(1);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
        camera.rotateZ(-1);
    }
}