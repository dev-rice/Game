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

    GLFWwindow* getGLFWWindow(){ return window;}

private:
    GLFWwindow* initializeGLFWWindow(int, int, bool);

    GLFWwindow* window;

};

#endif
