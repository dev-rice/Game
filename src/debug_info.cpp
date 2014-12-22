#include "debug_info.h"

DebugInfo::DebugInfo(GLFWwindow* window){
    text = new TextRenderer(window, "res/fonts/inconsolata_font.png", 0.015);
}

void DebugInfo::startFrame(){
    start_time = glfwGetTime();
}

void DebugInfo::endFrame(){
    float delta_t = glfwGetTime() - start_time;
    current_fps = 1.0 / delta_t;
}

void DebugInfo::draw(){
    char buffer[100];
    sprintf(buffer, "FPS: %f", current_fps);
    std::string to_print = buffer;
    text->drawString(glm::vec2(-0.9, 0.9), to_print);
}