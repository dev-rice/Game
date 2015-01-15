// ui_window.h
// Trevor Westphal

#ifndef UIWindow_h
#define UIWindow_h

#include "../texture_loader.h"
#include "ui_drawable.h"
#include "ui_image.h"
#include "ui_radiobutton.h"
#include "ui_button.h"

class UIWindow : public UIDrawable {
public:
    UIWindow(GLuint);

    void loadFromXML(std::string);

    void draw();
    void show();
    void hide();

    bool isShowing() {return is_showing;}
private:
    bool is_showing;

    std::vector<UIDrawable*> subelements;
};

#endif
