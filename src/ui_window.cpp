// ui_window.cpp
// Trevor Westphal

#include "ui_window.h"

UIWindow::UIWindow(FlatMesh* flat_mesh, GLuint shader_program) : UIDrawable(flat_mesh, shader_program, TextureLoader::loadGray()){

}
