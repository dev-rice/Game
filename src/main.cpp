#define GLEW_STATIC

#include <GL/glew.h>
#include <SFML/Window.hpp>

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

#include "debug.h"
#include "world.h"
#include "profile.h"
#include "window.h"
#include "mesh_loader.h"
#include "game_clock.h"

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

    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 0;

    // create the window
    sf::Window window(sf::VideoMode(width, height), "OpenGL", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    sf::Vector2u window_size = window.getSize();
    Debug::info("Window size: %d by %d\n", window_size.x, window_size.y);

    // Print various info about OpenGL
    Debug::info("Renderer:       %s\n", glGetString(GL_RENDERER));
    Debug::info("OpenGL version: %s\n", glGetString(GL_VERSION));
    Debug::info("GLSL version:   %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    Window* our_window = Window::getInstance();

    // Create the window
    our_window->setWidth(width);
    our_window->setHeight(height);
    our_window->setFullscreen(fullscreen);
    our_window->setVsync(vsync);
    our_window->initializeWindow();

    // Create the world
    World world;
    if (has_map){
        world = World(map_filename.c_str());
    } else {
        world = World();
    }

    // Display loop
    while(!our_window->shouldClose()) {
        // handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                // end the program
                our_window->requestClose();
            }
            else if (event.type == sf::Event::Resized) {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);

            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                our_window->requestClose();
            }
        }

        // draw...
        // glDrawArrays(GL_LINE_LOOP, 0, 3);
        world.update();

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // Close the window
    our_window->close();

    // Add a line break before going back to the terminal prompt.
    printf("\n");

    // Nothing went wrong!
    return 0;
}
