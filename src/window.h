#ifndef Window_h
#define Window_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
#endif

#include "debug.h"

class Window{
public:
    Window(int, int, bool);

    GLFWwindow* getGLFWWindow(){ return glfw_window;}

    void swapBuffers();

    int getWidth(){return width;}
    int getHeight(){return height;}

private:
    GLFWwindow* initializeGLFWWindow(int, int, bool);

    GLFWwindow* glfw_window;
    int width;
    int height;
};

#endif
