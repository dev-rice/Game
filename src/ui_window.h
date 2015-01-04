// ui_window.h
// Trevor Westphal

#ifndef UIWindow_h
#define UIWindow_h

#include "flat_drawable.h"
#include "window.h"
#include "ui_drawable.h"

class UIWindow : public UIDrawable {
public:
    UIWindow(FlatMesh*, Window*, GLuint, GLuint);

private:
};

#endif