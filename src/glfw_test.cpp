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
#include <iostream>
#include <vector>

#include "world.h"
#include "flat_mesh.h"
#include "flat_drawable.h"
#include "character_mesh.h"

GLFWwindow* initializeGLFWWindow(int, int, bool);

class Box {
   public:
      double length;   // Length of a box
      double breadth;  // Breadth of a box
      double height;   // Height of a box
};

int main(int argc, char* argv[]) {
    Box fucking_box;
    
    // Make the randomizer random
    srand(time(NULL));

    // Parse command line arguments
    float width;
    float height;
    bool fullscreen;
    
    glfwInit();

    if (argc == 2 && std::string(argv[1]) == "-f"){
        const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        width  = mode->width;
        height = mode->height;

        mode = NULL;
        delete mode;

        printf("Loading in fullscreen mode with resolution: %d by %d\n", (int)width, (int)height);
        fullscreen = true;
    } else if (argc == 4 && std::string(argv[1]) == "-w") {
        sscanf(argv[2], "%f", &width);
        sscanf(argv[3], "%f", &height);
        printf("Loading in windowed mode with resolution: %d by %d\n", (int)width, (int)height);
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
    World world(window);

    std::vector<float> all_fps;
    float last_time = glfwGetTime();

    //////////////////////////////////////////////////////////////////
    // Text shit

    GLuint text_vs = ShaderLoader::loadVertexShader("shaders/text.vs");
    GLuint text_fs = ShaderLoader::loadFragmentShader("shaders/text.fs");
    GLuint text_shader = ShaderLoader::combineShaderProgram(text_vs, text_fs);

    CharacterMesh* flat_mesh = new CharacterMesh();
    FlatDrawable character_box = FlatDrawable(flat_mesh, text_shader, 0.5, 0.5, glm::vec2(-0.5, 0.5));

    GLuint character_texture = TextureLoader::loadTextureFromFile("res/fonts/font_sheet.png", GL_NEAREST);

    character_box.attachTexture(character_texture);
    ////////////////////////////////////////////////////////////////// 


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

        float frame_time = glfwGetTime() - last_time;
        last_time = glfwGetTime();
        all_fps.push_back(1.0 / frame_time);

        world.update();

        character_box.draw();
            
        ////////////////////////////////////////////////////////////////
        // Find the mouse position in FlatDrawable coordinates
        double mouse_x;
        double mouse_y;

        glm::mat3 projection = glm::mat3( width / 2.0f, 0.0f         , width / 2.0f  ,
                                          0.0f        , -height / 2.0f, height / 2.0f,
                                          0.0f        , 0.0f         , 1.0f           );

        glfwGetCursorPos(window, &mouse_x, &mouse_y);

        glm::vec3 mouse_position = glm::vec3(mouse_x, mouse_y, 1.0);
        glm::vec3 gl_mouse_position = mouse_position * glm::inverse(projection);
        ////////////////////////////////////////////////////////////////

    }

    delete flat_mesh;
    flat_mesh = NULL;

    float total = 0;
    float min = all_fps[0];
    float max = all_fps[0];

    for (int i = 0; i < all_fps.size(); ++i){
        total += all_fps[i];
        if (all_fps[i] > max){
            max = all_fps[i];
        }
        if (all_fps[i] < min){
            min = all_fps[i];
        }
    }
    float average = total / all_fps.size();
    printf("Average FPS: %f\n", average);
    printf("Minimum FPS: %f\n", min);
    printf("Maximum FPS: %f\n", max);

    // Shut down GLFW before exiting the program
    glfwTerminate();

    // Nothing went wrong!
    return 0;
}

GLFWwindow* initializeGLFWWindow(int width, int height, bool fullscreen){
    // glfwInit();
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

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
    
    // Describe what constitutes the front face, and enable backface culling
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);

    // Alpha transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    // Enable MSAA
    glEnable(GL_MULTISAMPLE);

    return window;   
}