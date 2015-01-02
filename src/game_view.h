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
#include "profile.h"
#include "shadowbuffer.h"
#include "window.h"

class GameView {
public:

    GameView(Window*, Level*);

    void update();
private:
    void handleInputs();

    Window* window;
    GLFWwindow* glfw_window;

    Level* level;

    Framebuffer* screen;
    Framebuffer* framebuffer;
    Shadowbuffer* shadowbuffer;

    Mouse* mouse;
    std::vector<UIDrawable*> ui_drawables;

    TextRenderer* text_renderer;

    bool toggle_key_state;
    bool debug_showing;
};

#endif
