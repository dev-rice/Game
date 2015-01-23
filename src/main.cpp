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
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#include "world.h"
#include "debug.h"
#include "profile.h"
#include "window.h"
#include "mesh_loader.h"

void error_callback(int error, const char* description){
    Debug::error("GLFW Error %d: %s\n", error, description);
}

int main(int argc, char* argv[]) {

    // Make the randomizer random
    srand(time(NULL));

    // Parse command line arguments
    int fxaa_level = Profile::getInstance()->getFxaaLevel();

    float width  = Profile::getInstance()->getWindowWidth();
    float height = Profile::getInstance()->getWindowHeight();

    bool fullscreen  = !Profile::getInstance()->getWindowed();
    bool interactive = false;
    bool debug   = false;
    bool has_map = false;
    bool vsync   = Profile::getInstance()->getVsync();
    char argument;

    std::string map_filename;

    while ((argument = getopt(argc, argv, "wvfidm:x:")) != -1){
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
            std::string fxaa_level_str(optarg);
            fxaa_level = std::stoi(fxaa_level_str);
        } else if (argument == 'd'){
            debug = true;
        } else if (argument == 'm'){
            has_map = true;
            map_filename = std::string(optarg);
        } else if (argument == 'v'){
            vsync = true;
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
            printf("\t-v \n");
            printf("\t\tTurn on Verticl Sync.\n\n");

            return 1;
        }
    }

    Debug::is_on = debug;


    const char* glfw_version = glfwGetVersionString();
    Debug::info("GLFW Version: %s\n", glfw_version);
    if( !glfwInit() ) {
        Debug::error("Failed to initialize GLFW\n");
        return -1;
    }

    glfwSetErrorCallback(error_callback);

    if (fullscreen){
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        width  = mode->width;
        height = mode->height;

        Debug::info("Loading in fullscreen mode with resolution: %d by %d\n",
            (int)width, (int)height);
    } else {
        Debug::info("Loading in windowed mode with resolution: %d by %d\n",
            (int)width, (int)height);
    }

    Window* window = Window::getInstance();

    // Create the window
    window->setWidth(width);
    window->setHeight(height);
    window->setFullscreen(fullscreen);
    window->initializeWindow();

    window->setVsync(vsync);
    window->setFxaaLevel(fxaa_level);


    // Do some mesh loading
    MeshLoader mesh_loader = MeshLoader("res/models/sword.obj");
    mesh_loader.loadMeshFromDAE("res/models/cube.dae");

    // Create the world
    World* world;
    if (has_map){
        world = new World(map_filename.c_str());
    } else {
        world = new World();
    }

    // Display loop
    while(!window->shouldClose()) {
        world->update();
    }

    // Close the window
    window->close();

    delete world;
    world = NULL;

    // Add a line break before going back to the terminal prompt.
    printf("\n");

    // Nothing went wrong!
    return 0;
}
