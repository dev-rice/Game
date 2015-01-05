// ui_window.cpp
// Trevor Westphal

#include "ui_window.h"

UIWindow::UIWindow(FlatMesh* flat_mesh, Window* window, GLuint shader_program) : UIDrawable(flat_mesh, window, shader_program, TextureLoader::loadGray()){
    
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
            break;
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

    // Sizing the ui window
    int width = atoi(doc.first_node("layout")->first_node("dimensions")->first_node("width")->value());
    int height = atoi(doc.first_node("layout")->first_node("dimensions")->first_node("height")->value());

    setDimensions(width, height);

    // Positioning the ui window
    float x_position = 0;
    float y_position = 0;

    char* x_position_string = doc.first_node("layout")->first_node("dimensions")->first_node("x")->value();
    char* y_position_string = doc.first_node("layout")->first_node("dimensions")->first_node("y")->value();

    if(strcmp(x_position_string, "left") == 0){
        x_position = -1.0f;  
    } else if(strcmp(x_position_string, "centered") == 0){
        x_position = -1.0f * width/float(window_width);
    } else {
        x_position = 2.0f*(float(atoi(x_position_string))/float(window_width)) -1.0f;
    }

    if(strcmp(y_position_string, "top") == 0){
        y_position = 1.0f;  
    } else if(strcmp(y_position_string, "centered") == 0){
        y_position = height/float(window_height);
    } else {
        y_position = -2.0f*(float(atoi(y_position_string))/float(window_height)) + 1.0f;
    }

    setPosition(glm::vec2(x_position, y_position));


}

void UIWindow::setDimensions(int new_width, int new_height){
    glm::vec3 image_size = glm::vec3(new_width, new_height, 1.0);
    glm::vec3 gl_mesh_size = image_size * glm::inverse(mesh_projection);

    width = gl_mesh_size.x;
    height = gl_mesh_size.y;
}   
