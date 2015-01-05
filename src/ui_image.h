// ui_image.h
// Trevor Westphal

#ifndef UIImage_h
#define UIImage_h

#include "debug.h"
#include "flat_mesh.h"
#include "window.h"
#include "ui_drawable.h"

class UIImage : public UIDrawable {
public:
    UIImage(FlatMesh* flat_mesh, Window* window, GLuint shader_program, GLuint texture) : 
        UIDrawable(flat_mesh, window, shader_program, texture){;}
};

#endif