// ui_button.cpp
// Trevor Westphal

#include "ui_button.h"

UIImage* UIButton::center_image;
UIImage* UIButton::left_image;
UIImage* UIButton::right_image;

UIButton::UIButton(GLuint shader_program) : UIDrawable(shader_program, TextureLoader::loadPink()){

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
    height_pixels = 32;

    parseConstraints(constraints_node);

    // Add in decorative images
    if(center_image == NULL){
        center_image = new UIImage(shader, TextureLoader::loadTextureFromFile("res/textures/button_center.png", GL_NEAREST));
    }

    if(left_image == NULL){
        left_image = new UIImage(shader, TextureLoader::loadTextureFromFile("res/textures/button_left.png", GL_NEAREST));
    }
   
    if(right_image == NULL){
        right_image = new UIImage(shader, TextureLoader::loadTextureFromFile("res/textures/button_right.png", GL_NEAREST));
    }

    // Convert all pixel coords into screen
    updateDimensions();
    setGLPosition(getGLPosition());

    didLoadXML();
}   

bool UIButton::constraintsAreValid(bool x, bool y, bool w, bool h, bool x2, bool y2){
    return (x && y && w);
}

void UIButton::draw(){

    // A check to see if the radio button has been created properly
    if(center_image){

        // Draws pink bounding box. Useful for debugging
        // FlatDrawable::draw();

        center_image->setPositionAndDimensions(x_pixels + 15, y_pixels, width_pixels - 30, 32);
        left_image->setPositionAndDimensions(x_pixels - 1, y_pixels, 17, 32);
        right_image->setPositionAndDimensions(x_pixels + width_pixels - 16, y_pixels, 17, 32);
        
        center_image->draw();
        left_image->draw();
        right_image->draw();

    }

}