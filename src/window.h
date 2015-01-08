#ifndef Window_h
#define Window_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
#endif

#include <SOIL.h>
#include <ctime>
#include <string>

#include "debug.h"

class Window{
public:
    void initializeWindow();
    void swapBuffers();
    void close();
    void takeScreenshot();

    void setWidth(int w) { width = w;}
    void setHeight(int h) { height = h;}
    void setFullscreen(bool f) {fullscreen = f;}
    void setVsync(bool);
    void setFxaaLevel(int l) {fxaa_level = l;}

    int getWidth(){return width;}
    int getHeight(){return height;}
    int getFxaaLevel(){return fxaa_level;}
    GLFWwindow* getGLFWWindow(){ return glfw_window;}

    static Window* getInstance();

private:

    GLFWwindow* glfw_window;
    int width;
    int height;
    bool fullscreen;

    int requested_width;
    int requested_height;

    int fxaa_level;

    static Window* instance;
    Window();

};

#endif
