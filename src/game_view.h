#ifndef GameView_h
#define GameView_h

#include <ft2build.h>
#include FT_FREETYPE_H

#include "mesh.h"
#include "drawable.h"
#include "camera.h"
#include "texture_set.h"
#include "light.h"
#include "level.h"
#include "flat_mesh.h"
#include "flat_drawable.h"

class GameView {
public:
    
    GameView(GLFWwindow*, Level*);

    void update();
private:
    void handleInputs();

    GLFWwindow* window;
    Level* level;

    GLuint framebuffer;
    GLuint framebuffer_texture;

    FlatDrawable* framebuffer_window;
    GLuint basic_shader;

};

#endif