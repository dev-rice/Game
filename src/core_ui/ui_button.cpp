// ui_button.cpp
// Trevor Westphal

#include "ui_button.h"

UIButton::UIButton(GLuint shader_program) : UIDrawable(shader_program, TextureLoader::loadPink()){
    button_image = 0;
}

void UIButton::loadFromXML(std::string filepath){
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filepath.c_str());

    Debug::info("Parsed layout: %s\n", filepath.c_str());

    pugi::xml_node layout_node = doc.child("layout");

    if(strcmp(layout_node.child_value("type"), "ui_button") != 0){
        Debug::error("Mismatched layout types. Expected ui_button, received %s\n", layout_node.child_value("type"));
        return;
    }

    pugi::xml_node constraints_node = layout_node.child("constraints");

    // Parse function
    functionName = layout_node.child_value("function");

    // Parse button text
    text_renderer = new TextRenderer("res/fonts/inconsolata_bold_font.png", 28);
    button_text = layout_node.child_value("text");

    // Parse constraints
    width_pixels = 120;
    height_pixels = 28;

    parseConstraints(constraints_node);


    
}

bool UIButton::constraintsAreValid(bool x, bool y, bool w, bool h, bool x2, bool y2){
    return (x && y);
}

void UIButton::draw(){

}