#ifndef GameView_h
#define GameView_h

#include <ctime>
#include <vector>

#include "mesh.h"
#include "drawable.h"
#include "camera.h"
#include "level.h"
#include "flat_mesh.h"
#include "flat_drawable.h"
#include "text_renderer.h"
#include "ui_drawable.h"
#include "mouse.h"
#include "framebuffer.h"
#include "shadowbuffer.h"
#include "screenbuffer.h"
#include "window.h"
#include "ui_window.h"

class GameView {
public:

    GameView(Level*);

    void update();
private:
    void handleInputs();

    // Mouse controls
    int mouse_count;
    glm::vec2 initial_left_click_position;
    glm::vec2 final_left_click_position;

    Window* window;
    GLFWwindow* glfw_window;

    Level* level;

    Screenbuffer* screen;
    Framebuffer* framebuffer;
    Shadowbuffer* shadowbuffer;

    UIDrawable* selection_box;
    std::vector<UIDrawable*> ui_drawables;

    TextRenderer* text_renderer;

    bool toggle_key_state;
    bool debug_showing;

    // Move this to some fps/graphics debug class
    int frame_count;
    float average_frame_time;
};

#endif
