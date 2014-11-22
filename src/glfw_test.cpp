#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

#include <stdio.h>
#include <cstdlib>
#include <random>

#include "world.h"

GLFWwindow* initializeGLFWWindow(int, int, bool);

int main(int argc, char* argv[]) {
    // Make the randomizer random
    srand(time(NULL));

    // Parse command line arguments
    float width;
    float height;
    bool fullscreen;
    
    if (argc == 2 && std::string(argv[1]) == "-f"){
        width  = 1920;
        height = 1080;
        printf("Loading in fullscreen mode with resolution: %d by %d", (int)width, (int)height);
        fullscreen = true;
    } else if (argc == 4 && std::string(argv[1]) == "-w") {
        sscanf(argv[2], "%f", &width);
        sscanf(argv[3], "%f", &height);
        printf("Loading in windowed mode with resolution: %d by %d", (int)width, (int)height);
        fullscreen = false;
    } else {
        printf("\nCommand line options:\n");
        printf("\t-f\n");
        printf("\t\tRun in fullscreen mode\n");
        printf("\t-w <width> <height>\n");
        printf("\t\tRun in windowed mode with width and height\n\n");
        return 1;
    }

    // Create the window
    GLFWwindow* window = initializeGLFWWindow(width, height, fullscreen);

    // Create the world
    World world(width, height);

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

        // Clear the screen so we can draw again
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        world.handleInputs(window);
        world.update();
    }

    // Shut down GLFW before exiting the program
    glfwTerminate();

    // Nothing went wrong!
    return 0;
}

GLFWwindow* initializeGLFWWindow(int width, int height, bool fullscreen){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Set up the MSAA level 
    glfwWindowHint(GLFW_SAMPLES, 16);

    const char* windowTitle = "OpenGL";

    GLFWwindow* window;

    if (fullscreen){
        // Fullscreen 
        window = glfwCreateWindow(width, height, windowTitle, glfwGetPrimaryMonitor(),
            nullptr);

    } else {
        // Windowed
        window = glfwCreateWindow(width, height, windowTitle, nullptr, nullptr); 
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