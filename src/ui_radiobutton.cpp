// ui_radiobutton.cpp
// Trevor Westphal

#include "ui_radiobutton.h"

UIRadioButton::UIRadioButton(GLuint shader_program) : UIDrawable(shader_program, TextureLoader::loadPink()){
    this->shader = shader_program;
    this->has_clicked = false;
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

    // Parse function
    functionName = layout_node.child_value("function");

    // Parse radiobutton text
    text_renderer = new TextRenderer("res/fonts/inconsolata_bold_font.png", 12);
    radio_text = layout_node.child_value("text");

    // Parse constraints
    width_pixels = 80;
    height_pixels = 28;

    parseConstraints(constraints_node);

    // Set the initial value of the radio button
    radioButtonOn = (strcmp(layout_node.child("constraints").child_value("mode"), "on") == 0);

    // Create and position the button icon and hover highlight
    GLuint on_icon = TextureLoader::loadTextureFromFile("res/textures/radio_on.png", GL_NEAREST);
    on_icon_image = new UIImage(shader, on_icon, x_pixels, y_pixels, 28, 28);

    GLuint off_icon = TextureLoader::loadTextureFromFile("res/textures/radio_off.png", GL_NEAREST);
    off_icon_image = new UIImage(shader, off_icon, x_pixels, y_pixels, 28, 28);

    GLuint hover = TextureLoader::loadTextureFromFile("res/textures/radio_hover.png", GL_NEAREST);
    hoverIcon = new UIImage(shader, hover, x_pixels, y_pixels, 28, 28);

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

    currentIcon->draw();

    text_renderer->print(x_pixels, y_pixels, "%s", radio_text.c_str());

    glm::vec2 gl_mouse_position = Mouse::getInstance()->getGLPosition();
    if(gl_mouse_position.x < position.x + width &&
        gl_mouse_position.x > position.x - width &&
        gl_mouse_position.y < position.y + height &&
        gl_mouse_position.y > position.y - height){

        // FlatDrawable::draw();
        hoverIcon->draw();

        bool clicking = glfwGetMouseButton(Window::getInstance()->getGLFWWindow(), GLFW_MOUSE_BUTTON_LEFT);
        if(clicking && !has_clicked){
            toggleRadioButton();
            has_clicked = true;
        } else if(!clicking){
            has_clicked = false;
        }
    }

}

bool UIRadioButton::constraintsAreValid(bool x, bool y, bool w, bool h, bool x2, bool y2){
    return (x && y);
}

void UIRadioButton::toggleRadioButton(){

    FunctionHelper::getInstance()->runFunction(functionName);

    radioButtonOn = !radioButtonOn;

    if(radioButtonOn){
        currentIcon = on_icon_image;
    } else {
        currentIcon = off_icon_image;
    }
}
