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

#include <stdio.h>
#include <cstdlib>
#include <random>
#include <iostream>
#include <vector>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "debug.h"
#include "world.h"
#include "profile.h"
#include "window.h"
#include "mesh_loader.h"
#include "game_clock.h"
#include "input_handler.h"

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

    ///////////////////////////////////
    // Freetype stuff
    FT_Library library;
    FT_Face     face;
    int error;

    // Initialize the library
    error = FT_Init_FreeType( &library );
    if (error){
        Debug::error("Error initializing FreeType.\n");
    }

    // Load a font face
    const char* font_filename = "res/fonts/HelveticaNeueLight.ttf";
    error = FT_New_Face(library, font_filename, 0, &face);
    if (error == FT_Err_Unknown_File_Format){
        Debug::error("Invalid format of font file '%s'.\n", font_filename);
    } else if (error) {
        Debug::error("Error loading font face '%s'.\n", font_filename);
    }

    Debug::info("Face data:\n");
    Debug::info("  num_glyphs == %d\n", face->num_glyphs);
    Debug::info("  num_fixed_sizes == %d\n", face->num_fixed_sizes);

    int point_size = 16;
    error = FT_Set_Char_Size(face, 0, point_size * 64, width, height);
    if (error){
        Debug::error("Error setting face size to %d", point_size);
    }

    int glyph_index;
    char to_render = 'a';
    glyph_index = FT_Get_Char_Index(face, to_render);
    Debug::info("The character '%c' maps to glyph index %d\n", to_render, glyph_index);

    ///////////////////////////////////

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
