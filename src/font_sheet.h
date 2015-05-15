#ifndef FontSheet_h
#define FontSheet_h

#include "includes/gl.hpp"
#include "includes/sdl.hpp"
#include "includes/glm.hpp"
#include "includes/freetype.hpp"

#include <SOIL.h>

#include <algorithm>
#include <unordered_map>

#include "game_clock.h"
#include "debug.h"
#include "texture_loader.h"
#include "glyph.h"

static const std::string FONT_PATH = "res/fonts/";

class FontSheet {
public:
    static const int NUM_CHARS = 128;
    FontSheet(std::string filename, int pixel_size);

    void renderToBMP();

    GLuint getTexture();
    unsigned int getWidth();
    unsigned int getHeight();

    double getMaxYBearing();

    Glyph getGlyph(char);
    int getPointSize();

private:
    std::string filename;
    GLuint texture_id;

    unsigned int width;
    unsigned int height;
    double max_y_bearing;
    int point;

    std::unordered_map<char, Glyph> character_map;
};

#endif
