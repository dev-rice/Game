// ui_window.cpp
// Trevor Westphal

#include "ui_window.h"

UIWindow::UIWindow(GLuint shader_program) : UIDrawable(shader_program, TextureLoader::loadGray()){
    setDimensions(500, 500);
}

void UIWindow::loadFromXML(std::string filepath){

}
