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

#include <random>

#include "model.h"
#include "camera.h"
#include "drawable.h"

//////////////////////////
// Function prototypes
GLFWwindow* initializeGLFWWindow(int, int);

//////////////////////////
// Implementation
int main() {
    float width = 800.0f;
    float height = 600.0f;
    GLFWwindow* window = initializeGLFWWindow(width, height);


    Camera camera(0.0f, 0.0f, 2.0f);
    glm::mat4 proj_matrix = glm::perspective(45.0f, width / height, 1.0f, 100.0f);
    glm::mat4 view_matrix = glm::mat4();

    std::vector<Drawable> drawables;

    Model cube1 = Model();
    cube1.useTexture("res/diamond2.png");

    for (int i = -25; i < 25; ++i){
        for (int j = -25; j < 25; ++j){

            drawables.push_back(Drawable(&cube1, glm::vec3(i, j, rand() % 2)));
        }
    }
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

        // Camera controls
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            camera.z -= camera.sensitivity;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            camera.z += camera.sensitivity;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            camera.x += camera.sensitivity;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            camera.x -= camera.sensitivity;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
            camera.y -= camera.sensitivity;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
            camera.y += camera.sensitivity;
        }

        view_matrix = glm::lookAt(
            glm::vec3(camera.x, camera.y, camera.z),
            glm::vec3(camera.x, camera.y, -1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

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

GLFWwindow* initializeGLFWWindow(int width, int height){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Windowed
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", nullptr, nullptr); 
    // Fullscreen (Epilepsy mode)
    // GLFWwindow* window = glfwCreateWindow(1366, 768, "OpenGL", glfwGetPrimaryMonitor(), nullptr);

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

    return window;   
}