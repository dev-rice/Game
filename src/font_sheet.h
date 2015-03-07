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
