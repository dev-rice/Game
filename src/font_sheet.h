#ifndef FontSheetr_h
#define FontSheetr_h

#include <GL/glew.h>
#include <SDL2/SDL.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SOIL.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <algorithm>
#include <unordered_map>

#include "game_clock.h"
#include "debug.h"
#include "texture_loader.h"

static const std::string FONT_PATH = "res/fonts/";

struct Glyph {
    Glyph() {;}
    Glyph(double w, double h, double bx, double by, double ad){
        width = w / 64.0f;
        height = h / 64.0f;
        bearing_x = bx / 64.0f;
        bearing_y = by / 64.0f;
        advance = ad / 64.0f;
    }
    double width;
    double height;
    double bearing_x;
    double bearing_y;
    double advance;

    double u_offset;
    double v_offset;
};

class FontSheet {
public:
    static const int NUM_CHARS = 128;
    FontSheet(std::string filename, int pixel_size);

    void renderToBMP();

    GLuint getTexture();
    unsigned int getWidth();
    unsigned int getHeight();

    Glyph getGlyph(char);
    int getPointSize();

private:
    std::string filename;
    GLuint texture_id;

    unsigned int width;
    unsigned int height;
    int point;

    std::unordered_map<char, Glyph> character_map;
};

#endif
