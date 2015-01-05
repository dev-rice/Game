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

    void setDimensions(int new_width, int new_height){
        glm::vec3 image_size = glm::vec3(new_width, new_height, 1.0);
        glm::vec3 gl_mesh_size = image_size * glm::inverse(mesh_projection);

        width = gl_mesh_size.x;
        height = gl_mesh_size.y;
    }
};

#endif