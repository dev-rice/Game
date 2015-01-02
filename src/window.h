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
    static GLFWwindow* initializeGLFWWindow(int, int, bool);

private:
};

#endif
