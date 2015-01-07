// ui_window.cpp
// Trevor Westphal

#include "ui_window.h"

UIWindow::UIWindow(FlatMesh* flat_mesh, Window* window, GLuint shader_program) : UIDrawable(flat_mesh, window, shader_program, TextureLoader::loadGray()){
    this->game_window = window;
    this->shader_program = shader_program;
}

void UIWindow::loadFromXML(const char* filepath){
    // 1 kb buffer size for now
    char big_buffer[1024];
    char lil_buffer[128];
    int count = 0;

    FILE * ifile;
    ifile = fopen(filepath, "r");

    if(ifile == NULL){
        Debug::error("Error opening file %s\n", filepath);
        return;
    }

    while(! feof(ifile)){
        if(fgets(lil_buffer, 128, ifile) == NULL){
            // Can't read into buffer
            return;
        }

        for(int i = 0; lil_buffer[i] != '\0'; ++i, ++count){
            big_buffer[count] = lil_buffer[i];
        }
    }

    big_buffer[count] = '\0'; // Null terminating!
    fclose(ifile);

    rapidxml::xml_document<> doc; // character type defaults to char
    doc.parse<0>(big_buffer);     // 0 means default parse flags

    Debug::info("Parsed XML layout file '%s'\n", filepath);

    //##########################################
    // Sizing the UI Window
    //##########################################
    bool width_and_height_constrained = false;
    int width = 0;
    int height = 0;

    if(doc.first_node("layout")->first_node("constraints")->first_node("width") != NULL && doc.first_node("layout")->first_node("constraints")->first_node("height") != NULL){
        width = atoi(doc.first_node("layout")->first_node("constraints")->first_node("width")->value());
        height = atoi(doc.first_node("layout")->first_node("constraints")->first_node("height")->value());

        setDimensions(width, height);
        width_and_height_constrained = true;
    }

    //##########################################
    // Positioning the UI Window
    //##########################################
    int x_pos_int = 0;
    int y_pos_int = 0;

    char* x_anchor = doc.first_node("layout")->first_node("constraints")->first_node("x")->first_node("anchor")->value();
    char* y_anchor = doc.first_node("layout")->first_node("constraints")->first_node("y")->first_node("anchor")->value();

    int x_offset = atoi(doc.first_node("layout")->first_node("constraints")->first_node("y")->first_node("offset")->value());
    int y_offset = atoi(doc.first_node("layout")->first_node("constraints")->first_node("y")->first_node("offset")->value());

    x_pos_int = parseXConstraint(x_offset, x_anchor);
    y_pos_int = parseYConstraint(y_offset, y_anchor);

    setX(x_pos_int);
    setY(y_pos_int);

    float x_position = 2.0f*(float(x_pos_int)/float(window_width)) -1.0f;
    float y_position = -2.0f*(float(y_pos_int)/float(window_height)) + 1.0f;

    if(!width_and_height_constrained){
        int x2_pos_int = 0;
        int y2_pos_int = 0;

        char* x2_anchor = doc.first_node("layout")->first_node("constraints")->first_node("x")->next_sibling("x")->first_node("anchor")->value();
        char* y2_anchor = doc.first_node("layout")->first_node("constraints")->first_node("y")->next_sibling("y")->first_node("anchor")->value();

        int x2_offset = atoi(doc.first_node("layout")->first_node("constraints")->first_node("x")->next_sibling("x")->first_node("offset")->value());
        int y2_offset = atoi(doc.first_node("layout")->first_node("constraints")->first_node("y")->next_sibling("y")->first_node("offset")->value());

        x2_pos_int = parseXConstraint(x2_offset, x2_anchor);
        y2_pos_int = parseYConstraint(y2_offset, y2_anchor);

        width = x2_pos_int-x_pos_int;
        height = y2_pos_int-y_pos_int;
        setDimensions(width, height);
        setX(x_pos_int);
        setY(y_pos_int);
    }

    //##########################################
    // Creating and Positioning Edges
    //##########################################
    int edge_size = 18;
    int edge_offset_one = 8;
    int edge_offeset_two = 10;

    char* temp_filepath = doc.first_node("layout")->first_node("edge_sprites")->first_node("up")->value();
    UIImage* up = new UIImage(new FlatMesh(), game_window, shader_program, TextureLoader::loadTextureFromFile(temp_filepath, GL_NEAREST));
    up->setDimensions(width, edge_size);
    up->setPosition(glm::vec2(x_position, y_position + 2*(edge_offset_one/float(window_height))));
    sub_elements.push_back(up);

    temp_filepath = doc.first_node("layout")->first_node("edge_sprites")->first_node("right")->value();
    UIImage* right = new UIImage(new FlatMesh(), game_window, shader_program, TextureLoader::loadTextureFromFile(temp_filepath, GL_NEAREST));
    right->setDimensions(edge_size, height);
    right->setPosition(glm::vec2(x_position + 2*((width-edge_offeset_two)/float(window_width)), y_position));
    sub_elements.push_back(right);

    temp_filepath = doc.first_node("layout")->first_node("edge_sprites")->first_node("down")->value();
    UIImage* down = new UIImage(new FlatMesh(), game_window, shader_program, TextureLoader::loadTextureFromFile(temp_filepath, GL_NEAREST));
    down->setDimensions(width, edge_size);
    down->setPosition(glm::vec2(x_position, y_position - 2*((height-edge_offeset_two)/float(window_height))));
    sub_elements.push_back(down);

    temp_filepath = doc.first_node("layout")->first_node("edge_sprites")->first_node("left")->value();
    UIImage* left = new UIImage(new FlatMesh(), game_window, shader_program, TextureLoader::loadTextureFromFile(temp_filepath, GL_NEAREST));
    left->setDimensions(edge_size, height);
    left->setPosition(glm::vec2(x_position - 2*(edge_offset_one/float(window_width)), y_position));
    sub_elements.push_back(left);

    //##########################################
    // Creating and Positioning Corners
    //##########################################
    int corner_offset_one = 17;
    int corner_offset_two = 45;

    temp_filepath = doc.first_node("layout")->first_node("corner_sprites")->first_node("upper_left")->value();
    UIImage* up_left = new UIImage(new FlatMesh(), game_window, shader_program, TextureLoader::loadTextureFromFile(temp_filepath, GL_NEAREST));
    up_left->setPosition(glm::vec2(x_position - 2*(corner_offset_one/float(window_width)), y_position + 2*(corner_offset_one/float(window_height))));
    sub_elements.push_back(up_left);

    temp_filepath = doc.first_node("layout")->first_node("corner_sprites")->first_node("upper_right")->value();
    UIImage* up_right = new UIImage(new FlatMesh(), game_window, shader_program, TextureLoader::loadTextureFromFile(temp_filepath, GL_NEAREST));
    up_right->setPosition(glm::vec2(x_position + 2*((width-corner_offset_two)/float(window_width)), y_position + 2*(corner_offset_one/float(window_height))));
    sub_elements.push_back(up_right);

    temp_filepath = doc.first_node("layout")->first_node("corner_sprites")->first_node("lower_right")->value();
    UIImage* down_right = new UIImage(new FlatMesh(), game_window, shader_program, TextureLoader::loadTextureFromFile(temp_filepath, GL_NEAREST));
    down_right->setPosition(glm::vec2(x_position + 2*((width-corner_offset_two)/float(window_width)), y_position - 2*((height-corner_offset_two)/float(window_height))));
    sub_elements.push_back(down_right);

    temp_filepath = doc.first_node("layout")->first_node("corner_sprites")->first_node("lower_left")->value();
    UIImage* down_left = new UIImage(new FlatMesh(), game_window, shader_program, TextureLoader::loadTextureFromFile(temp_filepath, GL_NEAREST));
    down_left->setPosition(glm::vec2(x_position - 2*(corner_offset_one/float(window_width)), y_position - 2*((height-corner_offset_two)/float(window_height))));
    sub_elements.push_back(down_left);

}

void UIWindow::draw(){
    FlatDrawable::draw();

    for(int i = 0; i < sub_elements.size(); ++i){
        sub_elements[i]->draw();
    }

}

int UIWindow::parseXConstraint(int offset, char* anchor){
    if(strcmp(anchor, "left") == 0){
        return offset;
    } else if(strcmp(anchor, "centered") == 0){
        return window_width/2 - (width*window_width/2) + offset;
    } else if(strcmp(anchor, "right") == 0){
        return window_width + offset;
    } else if(strcmp(anchor, "none") == 0){
        return offset;
    }
    return -1;
}

int UIWindow::parseYConstraint(int offset, char* anchor){
    if(strcmp(anchor, "up") == 0){
        return offset;
    } else if(strcmp(anchor, "centered") == 0){
        return window_height/2 - (height*window_height/2) + offset;
    } else if(strcmp(anchor, "down") == 0){
        return window_height + offset;
    } else if(strcmp(anchor, "none") == 0){
        return offset;
    }
    return -1;
}

void UIWindow::setX(int new_x){
    position.x = 2.0f*(float(new_x)/float(window_width)) -1.0f + width;
}

void UIWindow::setY(int new_y){
    position.y = -2.0f*(float(new_y)/float(window_height)) + 1.0f - height;
}

void UIWindow::setDimensions(int new_width, int new_height){
    glm::vec3 image_size = glm::vec3(new_width, new_height, 1.0);
    glm::vec3 gl_mesh_size = image_size * glm::inverse(mesh_projection);

    width = gl_mesh_size.x;
    height = gl_mesh_size.y;
}
