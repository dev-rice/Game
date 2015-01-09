// ui_window.cpp
// Trevor Westphal

#include "ui_window.h"

UIWindow::UIWindow(GLuint shader_program) : UIDrawable(shader_program, TextureLoader::loadGray()){
    width_pixels = 500;
    height_pixels = 500;
}

void UIWindow::loadFromXML(std::string filepath){
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filepath.c_str());

    Debug::info("Parsed layout: %s\n", filepath.c_str());

    pugi::xml_node layout_node = doc.child("layout");

    if(strcmp(layout_node.child_value("type"), "ui_window") != 0){
        Debug::error("Mismatched layout types. Expected ui_window, received %s\n", layout_node.child_value("type"));
        return;
    }

    pugi::xml_node node = layout_node.child("type");
    // printf("%s", node.name());
    while(true){
        printf("%s\n", node.name());

        node = node.next_sibling();

        if(!node){
            break;
        }
    }
}

void UIWindow::draw(){

    updateDimensions();

    FlatDrawable::draw();

    // rememeber to draw sub elements!
}
