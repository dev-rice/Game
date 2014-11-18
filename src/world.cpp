#include "world.h"

World::World(float width, float height){
    camera = Camera(0.0f, 0.0f, 10.0f);
    
    view_matrix = glm::mat4();
    proj_matrix = glm::perspective(45.0f, width / height, 1.0f, 100.0f);

    GLuint vertex_shader = ShaderLoader::loadVertexShader("shaders/vertex_shader.glsl");
    GLuint fragment_shader = ShaderLoader::loadFragmentShader("shaders/fragment_shader.glsl");
    GLuint shader_program = ShaderLoader::combineShaderProgram(vertex_shader, fragment_shader);

    // Load models with textures. Textures should
    // load independently but for now you have to
    // keep track of the texture number even when
    // loading a new model
    Model cube = Model("res/models/lamppost.obj", 0.1f);
    cube.useTexture("res/textures/lamppost.png", GL_TEXTURE0);
    cube.attachShader(shader_program);

    Model ship = Model("res/models/gethtransport.obj", 1.0f);
    // ship.useTexture("res/textures/checkerboard.png", GL_TEXTURE1);
    ship.attachShader(shader_program);

    models.push_back(cube);
    models.push_back(ship);

    drawables.push_back(Drawable(&models[0], glm::vec3(0.0f, 0.0f, 0.0f)));
    drawables.push_back(Drawable(&models[1], glm::vec3(2.0f, 0.0f, 0.0f)));
}

void World::update(){
    // Update the view matrix based on the current
    // camera location / position
    view_matrix = camera.getViewMatrix();

    // Draw all the drawables
    for (int i = 0; i < drawables.size(); ++i){
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