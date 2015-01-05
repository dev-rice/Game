// ui_window.h
// Trevor Westphal

#ifndef UIWindow_h
#define UIWindow_h

#include <vector>
#include <cstdio> // NULL
#include <stdio.h>

#include "rapidxml.hpp"

#include "flat_drawable.h"
#include "debug.h"
#include "window.h"
#include "ui_drawable.h"
#include "texture_loader.h"
#include "ui_image.h"

class UIWindow : public UIDrawable {
public:
    UIWindow(FlatMesh*, Window*, GLuint);

    void loadFromXML(const char*);

    void draw();

protected:
    std::vector<UIDrawable*> sub_elements;

    void setDimensions(int, int);
    Window* game_window;
    GLuint shader_program;
};

#endif