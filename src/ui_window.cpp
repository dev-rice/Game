// ui_window.cpp
// Trevor Westphal

#include "ui_window.h"

UIWindow::UIWindow(GLuint shader_program) : UIDrawable(shader_program, TextureLoader::loadGray()){
    this->shader = shader_program;
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

    // Creating, Positioning, and Stretching the edges
    GLuint up_edge_sprite = TextureLoader::loadTextureFromFile(edge_sprites_node.child_value("up"), GL_NEAREST);
    UIImage *u = new UIImage(shader, up_edge_sprite, x_pixels, y_pixels - 8, width_pixels, 18);
    subelements.push_back(u);

    GLuint down_edge_sprite = TextureLoader::loadTextureFromFile(edge_sprites_node.child_value("down"), GL_NEAREST);
    u = new UIImage(shader, down_edge_sprite, x_pixels, y_pixels + height_pixels - 10, width_pixels, 18);
    subelements.push_back(u);

    GLuint right_edge_sprite = TextureLoader::loadTextureFromFile(edge_sprites_node.child_value("right"), GL_NEAREST);
    u = new UIImage(shader, right_edge_sprite, x_pixels + width_pixels - 10, y_pixels, 18, height_pixels);
    subelements.push_back(u);

    GLuint left_edge_sprite = TextureLoader::loadTextureFromFile(edge_sprites_node.child_value("left"), GL_NEAREST);
    u = new UIImage(shader, left_edge_sprite, x_pixels - 8, y_pixels, 18, height_pixels);
    subelements.push_back(u);

    // Creating and Positioning the corners
    GLuint up_left_corner_sprite = TextureLoader::loadTextureFromFile(corner_sprites_node.child_value("up_left"), GL_NEAREST);
    u = new UIImage(shader, up_left_corner_sprite, x_pixels - 17, y_pixels - 17, 62, 62);
    subelements.push_back(u);

    GLuint up_right_corner_sprite = TextureLoader::loadTextureFromFile(corner_sprites_node.child_value("up_right"), GL_NEAREST);
    u = new UIImage(shader, up_right_corner_sprite, x_pixels + width_pixels - 45, y_pixels - 17, 62, 62);
    subelements.push_back(u);

    GLuint down_right_corner_sprite = TextureLoader::loadTextureFromFile(corner_sprites_node.child_value("down_right"), GL_NEAREST);
    u = new UIImage(shader, down_right_corner_sprite, x_pixels + width_pixels - 45, y_pixels + height_pixels - 45, 62, 62);
    subelements.push_back(u);

    GLuint down_left_corner_sprite = TextureLoader::loadTextureFromFile(corner_sprites_node.child_value("down_left"), GL_NEAREST);
    u = new UIImage(shader, down_left_corner_sprite, x_pixels - 17, y_pixels + height_pixels - 45, 62, 62);
    subelements.push_back(u);
    

    for (pugi::xml_node_iterator it = subelements_node.begin(); it != subelements_node.end(); ++it){
         // printf("Named subelement: %s\n", it->name());

        UIDrawable* ui_element;
        
        if(strcmp(it->name(), "ui_radiobutton") == 0){
            ui_element = new UIRadioButton(shader);
            ui_element->loadFromXML(it->child_value("layout_filepath"));
            subelements.push_back(ui_element);
        }
    }

    // Convert all pixel coords into screen
    updateDimensions();
    setGLPosition(getGLPosition());
}

void UIWindow::draw(){

    FlatDrawable::draw();

    for(int i(0); i < subelements.size(); ++i){
        subelements[i]->draw();
    }
}

