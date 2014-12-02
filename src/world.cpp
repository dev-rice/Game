#include "world.h"

World::World(GLFWwindow* window, TextureContainer* textures){
    this->window = window;
    this->textures = textures;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    camera = Camera(0.0f, 0.0f, 10.0f);
    
    view_matrix = camera.getViewMatrix();
    proj_matrix = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);

    GLuint vertex_shader = ShaderLoader::loadVertexShader("shaders/vertex_shader.glsl");
    GLuint fragment_shader = ShaderLoader::loadFragmentShader("shaders/fragment_shader.glsl");
    GLuint shader_program = ShaderLoader::combineShaderProgram(vertex_shader, fragment_shader);

    Mesh raygun = Mesh("res/models/raygun.obj", 1.0f);
    raygun.attachTexture(textures->getTexture("raygun_diffuse.png"), DIFFUSE);
    raygun.attachTexture(textures->getTexture("raygun_specular.png"), SPECULAR);
    raygun.attachTexture(textures->getTexture("raygun_normal.png"), NORMAL);
    raygun.attachTexture(textures->getTexture("raygun_emissive.png"), EMISSIVE);
    raygun.attachShader(shader_program);

    Mesh other_gun = Mesh("res/models/raygun.obj", 1.0f);
    other_gun.attachTexture(textures->getTexture("base_diffuse.png"), DIFFUSE);
    other_gun.attachTexture(textures->getTexture("base_specular.png"), SPECULAR);
    other_gun.attachTexture(textures->getTexture("base_normal.png"), NORMAL);
    other_gun.attachTexture(textures->getTexture("base_emissive.png"), EMISSIVE);
    other_gun.attachShader(shader_program);

    meshes.push_back(raygun);
    meshes.push_back(other_gun);

    drawables.push_back(Drawable(&meshes[0], glm::vec3(0.0f, 0.0f, 0.0f)));
    drawables.push_back(Drawable(&meshes[1], glm::vec3(5.0f, 0.0f, 0.0f)));

}

void World::update(){
    // Update the view matrix based on the current
    // camera location / position
    view_matrix = camera.getViewMatrix();
    
    // Draw all the drawables
    for (int i = 0; i < drawables.size(); ++i){
        // This is how you move things
        // glm::vec3 position = drawables[i].getPosition();
        // drawables[i].moveTo(position + glm::vec3(-0.01f, 0.0f, 0.0f));
        
        drawables[i].draw(&view_matrix, &proj_matrix);
    }
}

void World::handleInputs(){
    glfwPollEvents();
    
    // Camera controls
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
}