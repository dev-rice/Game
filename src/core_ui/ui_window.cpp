// ui_window.cpp
// Trevor Westphal

#include "ui_window.h"

UIImage* UIWindow::up_edge_image;
UIImage* UIWindow::right_edge_image;
UIImage* UIWindow::left_edge_image;
UIImage* UIWindow::down_edge_image;

UIImage* UIWindow::up_left_corner_image;
UIImage* UIWindow::up_right_corner_image;
UIImage* UIWindow::down_left_corner_image;
UIImage* UIWindow::down_right_corner_image;

UIWindow::UIWindow(GLuint shader_program) : UIDrawable(shader_program, TextureLoader::loadGray()){
    this->shader = shader_program;
    is_showing = false;
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
    up_edge_image = new UIImage(shader, up_edge_sprite, x_pixels, y_pixels - 8, width_pixels, 18);

    GLuint down_edge_sprite = TextureLoader::loadTextureFromFile(edge_sprites_node.child_value("down"), GL_NEAREST);
    down_edge_image = new UIImage(shader, down_edge_sprite, x_pixels, y_pixels + height_pixels - 10, width_pixels, 18);

    GLuint right_edge_sprite = TextureLoader::loadTextureFromFile(edge_sprites_node.child_value("right"), GL_NEAREST);
    right_edge_image = new UIImage(shader, right_edge_sprite, x_pixels + width_pixels - 10, y_pixels, 18, height_pixels);

    GLuint left_edge_sprite = TextureLoader::loadTextureFromFile(edge_sprites_node.child_value("left"), GL_NEAREST);
    left_edge_image = new UIImage(shader, left_edge_sprite, x_pixels - 8, y_pixels, 18, height_pixels);

    // Creating and Positioning the corners
    GLuint up_left_corner_sprite = TextureLoader::loadTextureFromFile(corner_sprites_node.child_value("up_left"), GL_NEAREST);
    up_left_corner_image = new UIImage(shader, up_left_corner_sprite, x_pixels - 17, y_pixels - 17, 62, 62);

    GLuint up_right_corner_sprite = TextureLoader::loadTextureFromFile(corner_sprites_node.child_value("up_right"), GL_NEAREST);
    up_right_corner_image = new UIImage(shader, up_right_corner_sprite, x_pixels + width_pixels - 45, y_pixels - 17, 62, 62);

    GLuint down_right_corner_sprite = TextureLoader::loadTextureFromFile(corner_sprites_node.child_value("down_right"), GL_NEAREST);
    down_right_corner_image = new UIImage(shader, down_right_corner_sprite, x_pixels + width_pixels - 45, y_pixels + height_pixels - 45, 62, 62);

    GLuint down_left_corner_sprite = TextureLoader::loadTextureFromFile(corner_sprites_node.child_value("down_left"), GL_NEAREST);
    down_left_corner_image = new UIImage(shader, down_left_corner_sprite, x_pixels - 17, y_pixels + height_pixels - 45, 62, 62);


    for (pugi::xml_node_iterator it = subelements_node.begin(); it != subelements_node.end(); ++it){
         // printf("Named subelement: %s\n", it->name());

        UIDrawable* ui_element = 0;


        if(strcmp(it->name(), "ui_radiobutton") == 0){
            ui_element = new UIRadioButton(shader);
        } else if(strcmp(it->name(), "ui_button") == 0){
            ui_element = new UIButton(shader);
        }
        
        if(ui_element){
            ui_element->setParent(this);
            ui_element->loadFromXML(it->child_value("layout_filepath"));
            subelements.push_back(ui_element);
        }
    }

    // Convert all pixel coords into screen
    updateDimensions();
    setGLPosition(getGLPosition());
}

void UIWindow::draw(){
    if(is_showing){
        Mouse::getInstance()->setHovering();

        FlatDrawable::draw();

        up_edge_image->draw();
        right_edge_image->draw();
        left_edge_image->draw();
        down_edge_image->draw();

        up_left_corner_image->draw();
        up_right_corner_image->draw();
        down_left_corner_image->draw();
        down_right_corner_image->draw();

        for(int i(0); i < subelements.size(); ++i){
            subelements[i]->draw();
        }
    }
}

void UIWindow::show(){
    is_showing = true;
}

void UIWindow::hide(){
    is_showing = false;
}

