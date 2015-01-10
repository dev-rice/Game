// ui_radiobutton.cpp
// Trevor Westphal

#include "ui_radiobutton.h"

UIRadioButton::UIRadioButton(GLuint shader_program) : UIDrawable(shader_program, TextureLoader::loadPink()){
    this->shader = shader_program;
    this->mouse_count = 0;
}   

void UIRadioButton::loadFromXML(std::string filepath){
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filepath.c_str());

    Debug::info("Parsed layout: %s\n", filepath.c_str());

    pugi::xml_node layout_node = doc.child("layout");

    if(strcmp(layout_node.child_value("type"), "ui_radiobutton") != 0){
        Debug::error("Mismatched layout types. Expected ui_radiobutton, received %s\n", layout_node.child_value("type"));
        return;
    }

    pugi::xml_node constraints_node = layout_node.child("constraints");

    // Parse constraints
    parseConstraints(constraints_node);

    width_pixels = 80;
    height_pixels = 28;

    // Set the initial value of the radio button
    radioButtonOn = (strcmp(layout_node.child("constraints").child_value("mode"), "on") == 0);

    // Create and position the button icon
    GLuint on_icon = TextureLoader::loadTextureFromFile("res/textures/radio_on.png", GL_NEAREST);
    on_icon_image = new UIImage(shader, on_icon, x_pixels, y_pixels, 28, 28);

    GLuint off_icon = TextureLoader::loadTextureFromFile("res/textures/radio_off.png", GL_NEAREST);
    off_icon_image = new UIImage(shader, off_icon, x_pixels, y_pixels, 28, 28);

    if(radioButtonOn){
        currentIcon = on_icon_image;
    } else {
        currentIcon = off_icon_image;
    }

    // Convert all pixel coords into screen
    updateDimensions();
    setGLPosition(getGLPosition());
}

void UIRadioButton::draw(){

    FlatDrawable::draw();

    if(glfwGetMouseButton(Window::getInstance()->getGLFWWindow(), GLFW_MOUSE_BUTTON_LEFT)){
        mouse_count++;
    } 
    else if(mouse_count > 0){
        toggleRadioButton();
        mouse_count = 0;  
    }

    currentIcon->draw();

}

bool UIRadioButton::constraintsAreValid(bool x, bool y, bool w, bool h, bool x2, bool y2){
    return (x && y);
}

void UIRadioButton::toggleRadioButton(){
    radioButtonOn = !radioButtonOn;

    if(radioButtonOn){
        currentIcon = on_icon_image;
    } else {
        currentIcon = off_icon_image;
    }
}