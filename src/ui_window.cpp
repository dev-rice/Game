// ui_window.cpp
// Trevor Westphal

#include "ui_window.h"

UIWindow::UIWindow(GLuint shader_program) : UIDrawable(shader_program, TextureLoader::loadGray()){
    
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

    pugi::xml_node constraints_node = layout_node.child("constraints");
    pugi::xml_node corner_sprites_node = layout_node.child("corner_sprites");
    pugi::xml_node edge_sprites_node = layout_node.child("edge_sprites");
    pugi::xml_node subelements_node = layout_node.child("subelements");

    // Position and Sizing the Window
    parseConstraints(constraints_node);
    

    for (pugi::xml_node_iterator it = subelements_node.begin(); it != subelements_node.end(); ++it){
         printf("Named subelement: %s\n", it->name());
    }

    // Convert all pixel coords into screen
    updateDimensions();
    setGLPosition(getGLPosition());
}

void UIWindow::draw(){

    

    FlatDrawable::draw();

    // rememeber to draw sub elements!
}

