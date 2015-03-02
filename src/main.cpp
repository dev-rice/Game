#define GLEW_STATIC

#include <GL/glew.h>
#include <SDL2/SDL.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SOIL.h>

#include <stdio.h>
#include <cstdlib>
#include <random>
#include <iostream>
#include <vector>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "debug.h"
#include "world.h"
#include "profile.h"
#include "window.h"
#include "mesh_loader.h"
#include "game_clock.h"
#include "input_handler.h"
#include "font_sheet.h"

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

    Window* our_window = Window::getInstance();

    // Create the window
    our_window->setWidth(width);
    our_window->setHeight(height);
    our_window->setFullscreen(fullscreen);
    our_window->initializeWindow();

    // Create the world
    World* world;
    if (has_map){
        world = new World(map_filename.c_str());
    } else {
        world = new World();
    }

    // Start the input handler so we can use
    // it later.
    InputHandler::getInstance();

    FontSheet monospace("ShareTechMono-Regular.ttf", 32);
    FontSheet stylized("BreeSerif-Regular.ttf", 32);

    // Display loop
    while(!our_window->shouldClose()) {
        // Just handle inputs in this thread.
        InputHandler::getInstance()->pollInputs();
        world->update();
    }

    // Close the window
    our_window->close();

    delete world;
    world = NULL;

    // Add a line break before going back to the terminal prompt.
    printf("\n");

    // Nothing went wrong!
    return 0;
}
