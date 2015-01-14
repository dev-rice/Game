// ui_image.h
// Trevor Westphal

#ifndef UIIMAGE_h
#define UIIMAGE_h

#include "ui_drawable.h"

class UIImage : public UIDrawable {
public:
    UIImage(GLuint shader_program, GLuint texture, int x, int y, int width, int height) : UIDrawable(shader_program, texture){
        setPixelCoordinates(x, y, x+width, y+height);
    }
private:
};

#endif