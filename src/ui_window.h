// ui_window.h
// Trevor Westphal

#ifndef UIWindow_h
#define UIWindow_h

#include <string>   // std::string

#include "texture_loader.h"
#include "flat_drawable.h"
#include "window.h"
#include "ui_drawable.h"

class UIWindow : public UIDrawable {
public:
    UIWindow(GLuint);

    void loadFromXML(std::string);

    void draw();
private:
};

#endif
