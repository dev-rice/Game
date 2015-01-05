// ui_window.cpp
// Trevor Westphal

#include "ui_window.h"

UIWindow::UIWindow(FlatMesh* flat_mesh, Window* window, GLuint shader_program) : UIDrawable(flat_mesh, window, shader_program, TextureLoader::loadGray()){
    
}

void UIWindow::setDimensions(int new_width, int new_height){
    glm::vec3 image_size = glm::vec3(new_width, new_height, 1.0);
    glm::vec3 gl_mesh_size = image_size * glm::inverse(mesh_projection);

    width = gl_mesh_size.x;
    height = gl_mesh_size.y;
}   
