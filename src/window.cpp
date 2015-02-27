#include "window.h"

Window* Window::instance;

Window::Window(){
    should_close = false;
}

Window* Window::getInstance(){
    if(instance){
        return instance;
    } else {
        instance = new Window();
        return instance;
    }
}

void Window::display(){
    sfml_window->display();
}

void Window::requestClose() {
    Debug::info("Window close request received.\n");
    should_close = true;
}

bool Window::shouldClose(){
    return should_close;
}


void Window::close(){
}

void Window::setVsync(bool value){
    sfml_window->setVerticalSyncEnabled(value);
}

void Window::setWidth(int width){
    this->width = width;
}

void Window::setHeight(int height){
    this->height = height;
}

int Window::getWidth(){
    return width;
}

int Window::getHeight(){
    return height;
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

sf::Window* Window::getSFMLWindow(){
    return sfml_window;
}

void Window::initializeWindow(){
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 0;



    // create the window
    if (fullscreen){
        // Get all of the valid fullscreen graphics modes supported by your
        // graphics card and monitor
        std::vector<sf::VideoMode> fullscreen_modes = sf::VideoMode::getFullscreenModes();
        // Pick the best one
        sf::VideoMode fullscreen_mode = fullscreen_modes[0];

        // Update this window's width and height so that the framebuffers
        // are the correct size.
        width = fullscreen_mode.width;
        height = fullscreen_mode.height;

        Debug::info("Loading in fullscreen with resolution %d by %d\n", width, height);

        // Create the window
        sfml_window = new sf::Window(fullscreen_mode, "OpenGL", sf::Style::Fullscreen, settings);

    } else {
        Debug::info("Loading in windowed with resolution %d by %d\n", width, height);

        // Create the window
        sfml_window = new sf::Window(sf::VideoMode(width, height), "OpenGL", sf::Style::Default, settings);
    }

    setVsync(Profile::getInstance()->getVsync());
    sf::Vector2u window_size = sfml_window->getSize();
    Debug::info("Window size: %d by %d\n", window_size.x, window_size.y);

    // Hide the mouse because we have our own
    sfml_window->setMouseCursorVisible(false);

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

}
