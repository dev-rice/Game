#include "world.h"

World::World(GLFWwindow* window, std::vector<Mesh*> meshes, TextureContainer* textures){
    this->window = window;
    this->textures = textures;
    this->meshes = meshes;

    level = Level(window);
    level.loadLevel("res/maps/scene.map");

    // int width, height;
    // glfwGetFramebufferSize(window, &width, &height);

    // camera = Camera(glm::vec3(0.0f, 0.0f, 10.0f));
    
    // view_matrix = camera.getViewMatrix();
    // proj_matrix = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
    
    // light = Light(glm::vec3(3.0f, 2.5f, 2.0f), glm::vec3(1.0f, 0.9f, 0.9f), 10.0f);

    // TextureSet fence_textures;
    // fence_textures.diffuse = textures->getTexture(2);
    // fence_textures.specular = textures->getTexture(1);
    // fence_textures.normal = textures->getTexture(1);
    // fence_textures.emissive = textures->getTexture(1);

    // Drawable thefence = Drawable(meshes[0], glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    // thefence.attachTextureSet(fence_textures);
    // drawables.push_back(thefence);

}

void World::update(){
    // // Update the view matrix based on the current
    // // camera location / position
    // view_matrix = camera.getViewMatrix();
    
    // // Draw all the drawables
    // for (int i = 0; i < drawables.size(); ++i){
    //     // This is how you move things
    //     // glm::vec3 position = drawables[i].getPosition();
    //     // drawables[i].moveTo(position + glm::vec3(-0.01f, 0.0f, 0.0f));
        
    //     drawables[i].draw(&view_matrix, &proj_matrix, &light);
    // }
    level.handleInputs();
    level.draw();
}

void World::handleInputs(){
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