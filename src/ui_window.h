// ui_window.h
// Trevor Westphal

#ifndef UIWindow_h
#define UIWindow_h

#include <string>   // std::string
#include <vector>   // std::vector

#include "texture_loader.h"
#include "ui_drawable.h"
#include "ui_image.h"

class UIWindow : public UIDrawable {
public:
    UIWindow(GLuint);

    void loadFromXML(std::string);

    void draw();
private:
    std::vector<UIDrawable*> subelements;
};

#endif
