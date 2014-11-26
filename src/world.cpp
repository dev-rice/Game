#include "world.h"

World::World(float width, float height){
    camera = Camera(0.0f, 0.0f, 10.0f);
    
    view_matrix = glm::mat4();
    proj_matrix = glm::perspective(45.0f, width / height, 0.1f, 100.0f);

    GLuint vertex_shader = ShaderLoader::loadVertexShader("shaders/vertex_shader.glsl");
    GLuint fragment_shader = ShaderLoader::loadFragmentShader("shaders/fragment_shader.glsl");
    GLuint shader_program = ShaderLoader::combineShaderProgram(vertex_shader, fragment_shader);

    Mesh raygun = Mesh("res/models/raygun.obj", 1.0f);
    raygun.useTexture("res/textures/raygun_diffuse.png", GL_LINEAR, DIFFUSE);
    raygun.useTexture("res/textures/raygun_specular.png", GL_LINEAR, SPECULAR);
    raygun.useTexture("res/textures/raygun_normal.png", GL_LINEAR, NORMAL);
    raygun.useTexture("res/textures/raygun_emissive.png", GL_LINEAR, EMISSIVE);
    raygun.attachShader(shader_program);

    Mesh other_gun = Mesh("res/models/raygun.obj", 1.0f);
    other_gun.useTexture("res/textures/base_diffuse.png", GL_NEAREST, DIFFUSE);
    other_gun.useTexture("res/textures/base_specular.png", GL_LINEAR, SPECULAR);
    other_gun.useTexture("res/textures/base_normal.png", GL_LINEAR, NORMAL);
    other_gun.useTexture("res/textures/base_emissive.png", GL_LINEAR, EMISSIVE);
    other_gun.attachShader(shader_program);

    Mesh polycube = Mesh("res/models/manypoly.obj", 0.5f);
    polycube.useTexture("res/textures/base_diffuse.png", GL_NEAREST, DIFFUSE);
    polycube.useTexture("res/textures/base_specular.png", GL_LINEAR, SPECULAR);
    polycube.useTexture("res/textures/base_normal.png", GL_LINEAR, NORMAL);
    polycube.useTexture("res/textures/base_emissive.png", GL_LINEAR, EMISSIVE);
    polycube.attachShader(shader_program);

    meshes.push_back(raygun);
    meshes.push_back(other_gun);
    meshes.push_back(polycube);

    drawables.push_back(Drawable(&meshes[0], glm::vec3(0.0f, 0.0f, 0.0f)));
    drawables.push_back(Drawable(&meshes[1], glm::vec3(5.0f, 0.0f, 0.0f)));
    drawables.push_back(Drawable(&meshes[2], glm::vec3(2.5f, 2.0f, 0.0f)));

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

void World::handleInputs(GLFWwindow* window){
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