// ui_radiobutton.h
// Trevor Westphal

#ifndef UIRadioButton_h
#define UIRadioButton_h

#include "ui_drawable.h"
#include "texture_loader.h"
#include "ui_image.h"
#include "window.h"
#include "mouse.h"

class UIRadioButton : public UIDrawable {
public:
    UIRadioButton(GLuint);
    void loadFromXML(std::string);
    void draw();


private:
    bool constraintsAreValid(bool, bool, bool, bool, bool, bool);

    void toggleRadioButton();

    bool radioButtonOn;
    bool has_clicked;

    UIImage* on_icon_image;
    UIImage* off_icon_image;
    UIImage* hoverIcon;
    UIImage* currentIcon;
    // Text shit
};

#endif