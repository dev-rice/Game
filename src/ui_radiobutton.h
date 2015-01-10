// ui_radiobutton.h
// Trevor Westphal

#ifndef UIRadioButton_h
#define UIRadioButton_h

#include "ui_drawable.h"
#include "texture_loader.h"
#include "ui_image.h"
#include "window.h"

class UIRadioButton : public UIDrawable {
public:
    UIRadioButton(GLuint);
    void loadFromXML(std::string);
    void draw();


private:
    bool constraintsAreValid(bool, bool, bool, bool, bool, bool);

    void toggleRadioButton();

    bool radioButtonOn;
    bool mouse_count;

    UIImage* on_icon_image;
    UIImage* off_icon_image;
    UIImage* currentIcon;
    // Text shit
};

#endif