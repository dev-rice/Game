// ui_window.cpp
// Trevor Westphal

#include "ui_window.h"

UIWindow::UIWindow(FlatMesh* flat_mesh, Window* window, GLuint shader_program) : UIDrawable(flat_mesh, window, shader_program, TextureLoader::loadGray()){

}
