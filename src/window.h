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
    Window(int, int, bool);

    GLFWwindow* getGLFWWindow(){ return glfw_window;}

    void swapBuffers();
    void close();

    void setVsync(bool);
    void setFxaaLevel(int l) {fxaa_level = l;}

    void takeScreenshot();

    int getWidth(){return width;}
    int getHeight(){return height;}
    int getFxaaLevel(){return fxaa_level;}

private:
    GLFWwindow* initializeGLFWWindow(int, int, bool);

    GLFWwindow* glfw_window;
    int width;
    int height;

    int requested_width;
    int requested_height;

    int fxaa_level;
};

#endif
