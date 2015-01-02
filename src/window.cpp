#include "window.h"

Window::Window(int width, int height, bool fullscreen){
    this->width = width;
    this->height = height;
    glfw_window = initializeGLFWWindow(width, height, fullscreen);
}

void Window::swapBuffers(){
    glfwSwapBuffers(glfw_window);
}

GLFWwindow* Window::initializeGLFWWindow(int width, int height, bool fullscreen){
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    const char* windowTitle = "OpenGL";

    GLFWwindow* window;

    if (fullscreen){
        // Fullscreen
        window = glfwCreateWindow(width, height, windowTitle,
        glfwGetPrimaryMonitor(), nullptr);
    } else {
        // Windowed
        window = glfwCreateWindow(width, height, windowTitle, nullptr, nullptr);
    }

    // Set the cursor in the middle
    glfwSetCursorPos(window, width/2, height/2);

    int actual_w, actual_h;
    glfwGetFramebufferSize(window, &actual_w, &actual_h);
    if (actual_w != width || actual_h != height){
        Debug::warning("Actual render size is %d by %d.\n", actual_w, actual_h);
        this->width = actual_w;
        this->height = actual_h;
    }

    // Hide the mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Create the OpenGL context in the window
    glfwMakeContextCurrent(window);


    // Set up GLEW so that we can use abstracted OpenGL functions
    glewExperimental = GL_TRUE;
    glewInit();

    // Print various info about OpenGL
    Debug::info("Renderer:       %s\n", glGetString(GL_RENDERER));
    Debug::info("OpenGL version: %s\n", glGetString(GL_VERSION));
    Debug::info("GLSL version:   %s\n",
    glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Set up the correct depth rendering
    glEnable(GL_DEPTH_TEST);

    // Describe what constitutes the front face, and enable backface culling
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);

    // Alpha transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    // Turn off vsync
    // glfwSwapInterval(0);

    return window;
}
