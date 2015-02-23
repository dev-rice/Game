#include "window.h"

Window* Window::instance;

Window::Window(){
    should_close = false;
}

void Window::swapBuffers(){
    glfwSwapBuffers(glfw_window);
}

void Window::close(){
    glfwTerminate();
}

void Window::setVsync(bool value){
    if (value){
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }
}

void Window::takeScreenshot(){
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y_%m_%d-%H_%M_%S", &tstruct);

    std::string filename = "screenshot_" + std::string(buf) + ".bmp";

    // Create the unsigned byte array of size components * width * height
    // In this case components is 3 because we are saving the RGB components
    int components = 4;
    // Flipped vertically
    GLubyte* data = new GLubyte[components * width * height];
    glReadPixels(0, 0, width, height, GL_RGBA,
        GL_UNSIGNED_BYTE, data);

    // Flip the image about the horizontal axis
    GLubyte* correct_data = new GLubyte[components * width * height];
    int image_width = components * width;
    int image_height = height;
    for (int x = 0; x < image_width; x += components){
        for (int y = 0; y < image_height; ++y){
            for (int c = 0; c < components; ++c){
                int data_index = c + x + ((image_height - y - 1) * image_width);
                int correct_index = c + x + (y * image_width);

                GLubyte this_byte;
                if (c == 3){
                    // Set all alpha components to maximum value (-1)
                    // for unsigned byte
                    this_byte = -1;
                } else{
                    this_byte = data[data_index];
                }

                correct_data[correct_index] = this_byte;
            }
        }
    }

    int save_result = SOIL_save_image(filename.c_str(), SOIL_SAVE_TYPE_BMP,
        width, height, components, correct_data);
    if (save_result){
        Debug::info("Took screenshot %s.\n", filename.c_str());
    } else {
        Debug::error("Error saving screenshot %s.\n", filename.c_str());
    }

    delete[] data;
    delete[] correct_data;
    data = NULL;
    correct_data = NULL;
}

Window* Window::getInstance(){
    if(instance){
        return instance;
    } else {
        instance = new Window();
        return instance;
    }
}

void Window::initializeWindow(){
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

    if (!window){
        Debug::error("GLFW window creation failed.\n");
        glfwTerminate();
    }

    // Set the cursor in the middle
    glfwSetCursorPos(window, width/2, height/2);

    int actual_w, actual_h;
    glfwGetFramebufferSize(window, &actual_w, &actual_h);
    if (actual_w != width || actual_h != height){
        Debug::warning("Actual render size is %d by %d.\n", actual_w, actual_h);

    }

    this->width = actual_w;
    this->height = actual_h;

    // Hide the mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Create the OpenGL context in the window
    glfwMakeContextCurrent(window);

    // Sets the gl render resolution to the requested window resolution
    // glViewport(0, 0, width, height);


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

    glfw_window = window;
}
