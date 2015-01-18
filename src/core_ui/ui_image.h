// ui_image.h
// Trevor Westphal

#ifndef UIIMAGE_h
#define UIIMAGE_h

#include "ui_drawable.h"

class UIImage : public UIDrawable {
public:
    UIImage(GLuint shader_program, GLuint texture) : UIDrawable(shader_program, texture){
    }
    void setPositionAndDimensions(int x, int y, int width, int height){
    	 setPixelCoordinates(x, y, x+width, y+height);
    }
private:
};

#endif