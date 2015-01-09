// ui_window.cpp
// Trevor Westphal

#include "ui_window.h"

UIWindow::UIWindow(GLuint shader_program) : UIDrawable(shader_program, TextureLoader::loadGray()){
    width_pixels = 500;
    height_pixels = 500;
}

void UIWindow::loadFromXML(std::string filepath){

}

void UIWindow::draw(){

    updateDimensions();

    FlatDrawable::draw();

    // rememeber to draw sub elements!
}
