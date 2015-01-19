// ui_button.cpp
// Trevor Westphal

#include "ui_button.h"

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
    GLuint center = TextureLoader::loadTextureFromFile("res/textures/button_center.png", GL_NEAREST);
    // center_image = new UIImage(shader, center, x_pixels + 15, y_pixels, width_pixels - 30, 32);

    GLuint left = TextureLoader::loadTextureFromFile("res/textures/button_left.png", GL_NEAREST);
    // left_image = new UIImage(shader, left, x_pixels - 1, y_pixels, 17, 32);

    GLuint right = TextureLoader::loadTextureFromFile("res/textures/button_right.png", GL_NEAREST);
    // right_image = new UIImage(shader, right, x_pixels + width_pixels - 16, y_pixels, 17, 32);


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
        
        // center_image->draw();
        // left_image->draw();
        // right_image->draw();

    }

}