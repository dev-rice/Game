// ui_window.h
// Trevor Westphal

#ifndef UIWindow_h
#define UIWindow_h

#include <cstdio> // NULL
#include <stdio.h>

#include "rapidxml.hpp"

#include "flat_drawable.h"
#include "debug.h"
#include "window.h"
#include "ui_drawable.h"
#include "texture_loader.h"

class UIWindow : public UIDrawable {
public:
    UIWindow(FlatMesh*, Window*, GLuint);

    void loadFromXML(const char*);

protected:
    void setDimensions(int, int);

};

#endif