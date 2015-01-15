// ui_button.h
// Trevor Westphal

#ifndef UIButton_h
#define UIButton_h

#include "ui_drawable.h"
#include "../texture_loader.h"
#include "ui_image.h"
#include "../window.h"
#include "../mouse.h"
#include "../text_renderer.h"

class UIButton : public UIDrawable {
public:
    UIButton(GLuint);
    void loadFromXML(std::string);
    void draw();
    
private:  
};

#endif