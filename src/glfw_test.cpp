#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/OpenGL.h>

#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <thread>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <vector>
#include <time.h>

#include <random>

#include "model.h"
#include "camera.h"
#include "drawable.h"

//////////////////////////
// Function prototypes
GLFWwindow* initializeGLFWWindow(int, int, bool);
void handleCameraInputs(GLFWwindow*, Camera*);

//////////////////////////
// Implementation
int main(int argc, char* argv[]) {
    srand(time(NULL));

    float width;
    float height;
    bool fullscreen;
    
    if (argc == 2 && std::string(argv[1]) == "-f"){
        width  = 1366;
        height = 768;
        printf("Loading fullscreen with resolution: %f by %f", width, height);
        fullscreen = true;
    } else if (argc == 4 && std::string(argv[1]) == "-w") {
        sscanf(argv[2], "%f", &width);
        sscanf(argv[3], "%f", &height);
        fullscreen = false;
    } else {
        printf("\nCommand line options:\n");
        printf("\t-f\n");
        printf("\t\tRun in fullscreen mode\n");
        printf("\t-w <width> <height>\n");
        printf("\t\tRun in windowed mode with width and height\n\n");
        return 1;
    }

    GLFWwindow* window = initializeGLFWWindow(width, height, fullscreen);

    Camera camera(0.0f, 0.0f, 10.0f);
    glm::mat4 view_matrix = glm::mat4();
    glm::mat4 proj_matrix = glm::mat4();

    std::vector<Drawable> drawables;

    GLuint vertex_shader = ShaderLoader::loadVertexShader("shaders/vertex_shader.glsl");
    GLuint fragment_shader = ShaderLoader::loadFragmentShader("shaders/fragment_shader.glsl");
    GLuint shader_program = ShaderLoader::combineShaderProgram(vertex_shader, fragment_shader);

    // Load models with textures. Textures should
    // load independently but for now you have to
    // keep track of the texture number even when
    // loading a new model
    Model cube = Model("res/models/cubey.obj", 1.0f);
    cube.useTexture("res/textures/cubey.png", GL_TEXTURE0);
    cube.attachShader(shader_program);

    Model ship = Model("res/models/gethtransport.obj", 1.0f);
    ship.useTexture("res/textures/grass.jpeg", GL_TEXTURE1);
    ship.attachShader(shader_program);

    drawables.push_back(Drawable(&cube, glm::vec3(0.0f, 0.0f, 0.0f)));
    drawables.push_back(Drawable(&ship, glm::vec3(0.0f, 2.0f, 0.0f)));

    // Display loop
    while(!glfwWindowShouldClose(window)) {
        // Swap display/rendering buffers
        glfwSwapBuffers(window);
        
        // Handle events
        glfwPollEvents();
        // Check to see if escape is pressed. If so, close the window
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        handleCameraInputs(window, &camera);

        proj_matrix = glm::perspective(field_of_view, width / height, 1.0f, 100.0f);
        view_matrix = camera.getViewMatrix();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < drawables.size(); ++i){
            drawables[i].draw(&view_matrix, &proj_matrix);
        }
    }

    // Shut down GLFW before exiting the program
    glfwTerminate();

    // Nothing went wrong!
    return 0;
}

void handleCameraInputs(GLFWwindow* window, Camera* camera){
    // Camera controls
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
}

GLFWwindow* initializeGLFWWindow(int width, int height, bool fullscreen){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Set up the MSAA level 
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window;

    if (fullscreen){
        // Fullscreen 
        window = glfwCreateWindow(width, height, "OpenGL", glfwGetPrimaryMonitor(),
            nullptr);

    } else {
        // Windowed
        window = glfwCreateWindow(width, height, "OpenGL", nullptr, nullptr); 
    }

    // Hide the mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Create the OpenGL context in the window
    glfwMakeContextCurrent(window);

    // Set up GLEW so that we can use abstracted OpenGL functions
    glewExperimental = GL_TRUE;
    glewInit();

    // Print various info about OpenGL
    printf("\n");
    printf("Renderer:       %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version: %s\n", glGetString(GL_VERSION));
    printf("GLSL version:   %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("\n");

    // Set up the correct depth rendering 
    glEnable(GL_DEPTH_TEST);
    
    // Set up back-face culling maybe?
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    // Enable MSAA
    glEnable(GL_MULTISAMPLE);

    return window;   
}