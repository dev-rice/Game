#include "ui_drawable.h"

UIDrawable::UIDrawable(GLuint shader_program, GLuint texture) : FlatDrawable(shader_program){
    window_width = Window::getInstance()->getWidth();
    window_height = Window::getInstance()->getHeight();

    glm::mat3 mesh_projection = glm::mat3( window_width, 0.0f  , 0.0f,
                                      0.0f , window_height, 0.0f,
                                      0.0f , 0.0f  , 1.0f );

    inv_mesh_projection = glm::inverse(mesh_projection);

    attachTexture(texture);

}

void UIDrawable::attachTexture(GLuint texture){
    glBindTexture(GL_TEXTURE_2D, texture);
    int miplevel = 0;
    int w, h;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);

    glm::vec3 image_size = glm::vec3(w, h, 1.0);
    glm::vec3 gl_mesh_size = image_size * inv_mesh_projection;

    width = gl_mesh_size.x;
    height = gl_mesh_size.y;

    FlatDrawable::attachTexture(texture);
}

glm::vec2 UIDrawable::getGLPosition(){
    return glm::vec2( 2.0f*(float(x_pixels)/float(window_width))-1.0f, -2.0f*(float(y_pixels)/float(window_height))+1.0f);
}

void UIDrawable::setGLPosition(glm::vec2 position){
    this->position = glm::vec2(position.x + width, position.y - height);
}

void UIDrawable::draw(){
    FlatDrawable::draw();

    if (outline){
        glUniform1i(glGetUniformLocation(shader_program, "is_outline"), true);
        mesh->drawOutline();
        glUniform1i(glGetUniformLocation(shader_program, "is_outline"), false);
    }
}

void UIDrawable::setCoordinates(glm::vec2 start, glm::vec2 end){
    this->width = (end.x - start.x)/2;
    this->height = (end.y - start.y)/2;
    this->position = glm::vec2(start.x + width, start.y + height);
}

void UIDrawable::updateDimensions(){
    // Don't use me for things that WON'T change scale
    // like mice, etc
    width = (float(width_pixels)/float(window_width));
    height = (float(height_pixels)/float(window_height));
}

void UIDrawable::parseConstraints(pugi::xml_node constraints_node){
    
    bool has_x_position = constraints_node.child("x");
    bool has_y_position = constraints_node.child("y");
    bool has_width = constraints_node.child("width");
    bool has_height = constraints_node.child("height");
    bool has_x2_position = constraints_node.child("x2");
    bool has_y2_position = constraints_node.child("y2");

    // Must have x and y position. Then, must have x2 OR width but not both, and y2 OR height but not both
    if(has_x_position && has_y_position && XOR(has_width, has_x2_position) && XOR(has_height, has_y2_position)){
        
        const char* x_anchor = constraints_node.child("x").child_value("anchor");
        int x_offset = atoi(constraints_node.child("x").child_value("offset"));
        x_pixels = parseAnchor(x_anchor, true) + x_offset;
        
        const char* y_anchor = constraints_node.child("y").child_value("anchor");
        int y_offset = atoi(constraints_node.child("y").child_value("offset"));
        y_pixels = parseAnchor(y_anchor, false) + y_offset;

    } else {
        Debug::error("Incorrect constraints:\n [%d] X position\n [%d] Y position\n [%d] Width\n [%d] Height\n [%d] X2 position\n [%d] Y2 position\n", 
            has_x_position, has_y_position, has_width, has_height, has_x2_position, has_y2_position);
    }
}

bool UIDrawable::XOR(bool a, bool b){
    return (!a != !b);
}

int UIDrawable::parseAnchor(const char* anchor, bool is_x){
    if(strcmp(anchor, "left") == 0 || strcmp(anchor, "up") == 0){
        return 0;
    }
    if(strcmp(anchor, "centered") == 0){
        if(is_x){
            return window_width/2;
        }
        return window_height/2;
    }
    if(strcmp(anchor, "right") == 0 || strcmp(anchor, "down") == 0){
        if(is_x){
            return window_width;
        }
        return window_height;
    }

    return 0; // Error
}

