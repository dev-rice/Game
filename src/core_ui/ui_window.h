// ui_window.h
// Trevor Westphal

#ifndef UIWindow_h
#define UIWindow_h

#include "texture_loader.h"
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

    static UIImage* up_edge_image;
    static UIImage* right_edge_image;
    static UIImage* left_edge_image;
    static UIImage* down_edge_image;

    int up_edge_offset_x;
    int right_edge_offset_x;
    int left_edge_offset_x;
    int down_edge_offset_x;

    int up_edge_offset_y;
    int right_edge_offset_y;
    int left_edge_offset_y;
    int down_edge_offset_y;

    int up_edge_width;
    int right_edge_width;
    int left_edge_width;
    int down_edge_width;

    int up_edge_height;
    int right_edge_height;
    int left_edge_height;
    int down_edge_height;

    static UIImage* up_left_corner_image;
    static UIImage* up_right_corner_image;
    static UIImage* down_left_corner_image;
    static UIImage* down_right_corner_image;

    int up_left_corner_offset_x;
    int up_right_corner_offset_x;
    int down_left_corner_offset_x;
    int down_right_corner_offset_x;

    int up_left_corner_offset_y;
    int up_right_corner_offset_y;
    int down_left_corner_offset_y;
    int down_right_corner_offset_y;

    int up_left_corner_width;
    int up_right_corner_width;
    int down_left_corner_width;
    int down_right_corner_width;

    int up_left_corner_height;
    int up_right_corner_height;
    int down_left_corner_height;
    int down_right_corner_height;
};

#endif
