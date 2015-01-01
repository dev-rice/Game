#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <cstdlib>
#include <random>
#include <iostream>
#include <vector>
// #include <thread>         // std::thread
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#include "world.h"
#include "debug.h"
#include "profile.h"
#include "mesh.h"
#include "drawable.h"
#include "texture_loader.h"
#include "shader_loader.h"
#include "texture_set.h"
#include "framebuffer.h"
#include "shadowbuffer.h"

void renderEverything(GLFWwindow*);
void inputConsole();
GLFWwindow* initializeGLFWWindow(int, int, bool);

int main(int argc, char* argv[]) {
    // Make the randomizer random
    srand(time(NULL));

    // Parse command line arguments
    float width;
    float height;
    bool fullscreen = false;
    bool interactive = false;
    bool debug = false;
    char argument;
    bool has_map;
    std::string map_filename;

    while ((argument = getopt(argc, argv, "wfidm:x:")) != -1){
        // printf("Read command line option:\n");
        // printf("  argument = %c\n", argument);
        // printf("  optopt   = %c\n", optopt);
        // printf("  optarg   = %s\n\n", optarg);

        if (argument == 'f'){
            fullscreen = true;
        } else if (argument == 'w'){
            fullscreen = false;
        } else if (argument == 'i'){
            interactive = true;
        } else if (argument == 'x'){
            Profile::fxaa = true;
            std::string fxaa_level_str(optarg);
            Profile::fxaa_level = std::stoi(fxaa_level_str);
        } else if (argument == 'd'){
            debug = true;
        } else if (argument == 'm'){
            has_map = true;
            map_filename = std::string(optarg);
        } else {
            printf("\nCommand line options:\n");
            printf("\t-f\n");
            printf("\t\tRun in fullscreen mode.\n");
            printf("\t-w\n");
            printf("\t\tRun in windowed mode.\n");
            printf("\t-i\n");
            printf("\t\tRun in interactive mode.\n");
            printf("\t-d\n");
            printf("\t\tShow the debug log.\n\n");
            printf("\t-m <map_filename>\n");
            printf("\t\tLoad the world with map <map_filename>.\n\n");
            printf("\t-x <level>\n");
            printf("\t\tTurn on FXAA with level <level>.\n\n");

            return 1;
        }
    }

    Debug::is_on = debug;

    glfwInit();
    if (fullscreen){
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        width  = mode->width;
        height = mode->height;

        Debug::info("Loading in fullscreen mode with resolution: %d by %d\n",
            (int)width, (int)height);
    } else {
        width = 1600;
        height = 900;
        Debug::info("Loading in windowed mode with resolution: %d by %d\n",
            (int)width, (int)height);
    }

    // Create the window
    GLFWwindow* window = initializeGLFWWindow(width, height, fullscreen);

    // // Create the world
    // World* world;
    // if (has_map){
    //     world = new World(window, map_filename.c_str());
    // } else {
    //     world = new World(window);
    // }
    Framebuffer* screen = new Framebuffer();
    Framebuffer* scene = new Framebuffer(window);
    Framebuffer* shadowbuffer = new Shadowbuffer(2048, 2048);

    Mesh* castle_mesh = new Mesh("res/models/castle_tower.obj");
    Mesh* fence_mesh = new Mesh("res/models/fence.obj");

    GLuint shadow_vs = ShaderLoader::loadVertexShader("shaders/shadow.vs");
    GLuint shadow_fs = ShaderLoader::loadFragmentShader("shaders/shadow.fs");
    GLuint shadow_shader = ShaderLoader::combineShaderProgram(shadow_vs, shadow_fs);

    GLuint doodad_vs = ShaderLoader::loadVertexShader("shaders/doodad.vs");
    GLuint doodad_fs = ShaderLoader::loadFragmentShader("shaders/doodad.fs");
    GLuint doodad_shader = ShaderLoader::combineShaderProgram(doodad_vs, doodad_fs);

    Drawable* castle = new Doodad(castle_mesh, shadow_shader);
    GLuint diffuse = TextureLoader::loadTextureFromFile("res/textures/castle_tower_diff.png", GL_LINEAR);
    TextureSet textures(diffuse, 0, 0, 0);
    castle->attachTextureSet(textures);

    Drawable* fence = new Doodad(fence_mesh, shadow_shader, glm::vec3(0.0, 2.0, 4.0), glm::vec3(0.0f, 0.7f, 0.0f), 1.0f);
    diffuse = TextureLoader::loadTextureFromFile("res/textures/fence_diff.png", GL_LINEAR);
    textures = TextureSet(diffuse, 0, 0, 0);
    fence->attachTextureSet(textures);

    glm::vec3 lightInvDir = glm::vec3(0.5f, 2.0f, 2.0f);
    glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-10,20);
    glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,1,0));

    glm::mat4 proj_matrix = glm::perspective(45.0f, width / height, 0.1f, 500.0f);

    Camera* camera = new Camera();

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
        shadowbuffer->setAsRenderTarget();
        castle->setShader(shadow_shader);
        fence->setShader(shadow_shader);
        castle->draw(&depthViewMatrix, &depthProjectionMatrix);
        fence->draw(&depthViewMatrix, &depthProjectionMatrix);

        scene->setAsRenderTarget();
        // castle->setShader(doodad_shader);
        // fence->setShader(doodad_shader);
        // castle->draw(camera, &proj_matrix);
        // fence->draw(camera, &proj_matrix);

        screen->setAsRenderTarget();
        scene->draw();
        shadowbuffer->draw();
        // world->update();
    }

    // Kill glfw to end the program
    glfwTerminate();

    // delete world;
    // world = NULL;

    // Nothing went wrong!
    return 0;
}

void inputConsole(){
    std::string input;
    while(input != "quit"){
        char buffer[128];
        printf("> ");
        scanf(" %[^\n]s", buffer);
        printf("  '%s' is not a recognized command.\n", buffer);
        input = buffer;
    }
}

GLFWwindow* initializeGLFWWindow(int width, int height, bool fullscreen){
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Set up the MSAA level
    // glfwWindowHint(GLFW_SAMPLES, 16);

    const char* windowTitle = "OpenGL";

    GLFWwindow* window;

    if (fullscreen){
        // Fullscreen
        window = glfwCreateWindow(width, height, windowTitle,
            glfwGetPrimaryMonitor(), nullptr);
    } else {
        // Windowed
        window = glfwCreateWindow(width, height, windowTitle, nullptr, nullptr);
    }

    int actual_w, actual_h;
    glfwGetFramebufferSize(window, &actual_w, &actual_h);
    if (actual_w != width || actual_h != height){
        Debug::warning("Actual render size is %d by %d.\n", actual_w, actual_h);
    }

    // Hide the mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Create the OpenGL context in the window
    glfwMakeContextCurrent(window);

    // Set up GLEW so that we can use abstracted OpenGL functions
    glewExperimental = GL_TRUE;
    glewInit();

    // Print various info about OpenGL
    Debug::info("Renderer:       %s\n", glGetString(GL_RENDERER));
    Debug::info("OpenGL version: %s\n", glGetString(GL_VERSION));
    Debug::info("GLSL version:   %s\n",
        glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Set up the correct depth rendering
    glEnable(GL_DEPTH_TEST);

    // Describe what constitutes the front face, and enable backface culling
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);

    // Alpha transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    // Enable MSAA
    // glEnable(GL_MULTISAMPLE);

    return window;
}
