#ifndef Window_h
#define Window_h

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SDL2/SDL.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SOIL.h>
#include <ctime>
#include <string>

#include "debug.h"
#include "profile.h"

class Window{
public:
    static Window* getInstance();

    void initializeWindow();
    void display();
    void close();
    void takeScreenshot();

    void requestClose();
    bool shouldClose();

    void setWidth(int w);
    void setHeight(int h);
    void setFullscreen(bool f) {fullscreen = f;}
    void setVsync(bool);

    int getWidth();
    int getHeight();

    float getWidthScale(){return width_scale;}
    float getHeightScale(){return height_scale;}

    sf::Window* getSFMLWindow();

private:

    int width;
    int height;
    bool fullscreen;

    float width_scale;
    float height_scale;

    int requested_width;
    int requested_height;

    bool should_close;

    sf::Window* sfml_window;

    SDL_Window* sdl_window;
    SDL_GLContext gl_context;

    static Window* instance;
    Window();

};

#endif
