#ifndef GameView_h
#define GameView_h

#include <ctime>
#include <vector>
#include <thread>
#include <mutex>

#include "mesh.h"
#include "drawable.h"
#include "camera.h"
#include "level.h"
#include "flat_mesh.h"
#include "flat_drawable.h"
#include "text_renderer.h"
#include "core_ui/ui_drawable.h"
#include "mouse.h"
#include "framebuffer.h"
#include "shadowbuffer.h"
#include "screenbuffer.h"
#include "window.h"
#include "core_ui/ui_window.h"
#include "pathfinder.h"
#include "game_clock.h"
#include "debug_console.h"

class GameView {
public:

    GameView(Level*);
    ~GameView();

    void update();
private:
    void handleInputs();

    // Mouse controls
    int mouse_count;
    bool left_mouse_button_unclick;
    glm::vec2 initial_left_click_position;
    glm::vec2 final_left_click_position;

    bool middle_mouse_button_click;

    bool right_mouse_button_click;
    bool right_mouse_button_unclick;

    bool attack_command_prime;

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

    bool printscreen_key_state;

    UIWindow* menu;
    bool menu_key_state;

    UIWindow* graphics_menu;
    bool graphics_menu_key_state;

    DebugConsole* debug_console;
    bool debug_console_key_state;

    GLuint mouse_ubo;

    std::thread input_thread;
};

#endif
