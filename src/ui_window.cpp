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
    // update width and height. Move this to a helper
    width = (float(width_pixels)/float(window_width));
    height = (float(height_pixels)/float(window_height));

    FlatDrawable::draw();

    // rememeber to draw sub elements!
}
