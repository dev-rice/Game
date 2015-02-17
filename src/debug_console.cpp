#include "debug_console.h"

std::mutex mtx;

DebugConsole::DebugConsole(GLuint shader_program) : UIWindow(shader_program) {
    loadFromXML("res/layouts/debug_console.xml");

    text_renderer = new TextRenderer("res/fonts/inconsolata_bold_font.png", 22);

    input_buffer = "Hello, world!";
    input_thread = std::thread(&DebugConsole::pollInputs, this);
}

void DebugConsole::draw(){
    if (is_showing){
        UIWindow::draw();
        const char* temp_buffer = input_buffer.c_str();
        text_renderer->print(x_pixels + 16, y_pixels + 16, temp_buffer);
    }

}

void DebugConsole::pollInputs(){
    GLFWwindow* glfw_window = Window::getInstance()->getGLFWWindow();
    while (true){
        if (glfwGetKey(glfw_window, GLFW_KEY_A) == GLFW_PRESS){
            input_buffer = "a";
        }
    }
}
