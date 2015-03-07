#include "font_sheet.h"

FontSheet::FontSheet(std::string filename, int pixel_size) {
    this->filename = filename;
    this->point = pixel_size;

    std::string font_filename = FONT_PATH + filename;

    FT_Library library;
    FT_Face     face;
    int error;

    // Initialize the library
    error = FT_Init_FreeType( &library );
    if (error){
        Debug::error("Error initializing FreeType.\n");
    }

    // Load a font face
    error = FT_New_Face(library, font_filename.c_str(), 0, &face);
    if (error == FT_Err_Unknown_File_Format){
        Debug::error("Invalid format of font file '%s'.\n", font_filename.c_str());
        return;
    } else if (error) {
        Debug::error("Error loading font face '%s'.\n", font_filename.c_str());
        return;
    }

    error = FT_Set_Pixel_Sizes(face, 0, pixel_size);
    if (error){
        Debug::error("Cannot set face size to %d", pixel_size);
        return;
    }

    width = NUM_CHARS * pixel_size;
    height = pixel_size;

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLubyte* zeros = new GLubyte[width * height];
    for (int i = 0; i < width * height; ++i){
        zeros[i] = 0;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, zeros);

    delete[] zeros;
    zeros = NULL;

    // Set the texture wrapping to clamp to edge
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Do nearest interpolation for scaling the image up and down.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Mipmaps increase efficiency or something
    glGenerateMipmap(GL_TEXTURE_2D);

    float start_time = GameClock::getInstance()->getCurrentTime();

    int x_offset = 0;
    int y_offset = 0;

    max_y_bearing = 0;

    for (int i = 0; i < NUM_CHARS; ++i){
        char to_render = i;

        int load_flags = FT_LOAD_RENDER;
        error = FT_Load_Char(face, to_render, load_flags);
        if (error){
            Debug::error("Cannot load glyph %d.\n", to_render);
        }

        FT_GlyphSlot glyph = face->glyph;

        x_offset += pixel_size;

        glTexSubImage2D(GL_TEXTURE_2D, 0, x_offset, 0, glyph->bitmap.width,
            glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);

        glm::vec2 uv_offset = glm::vec2((double)x_offset / (double)width, 0);
        Glyph current_glyph(glyph, uv_offset);

        character_map[to_render] = current_glyph;

        // Keep track of the maximum y bearing so that we can use it for
        // alignment later.
        max_y_bearing = std::fmax(max_y_bearing, current_glyph.getBearingY());

    }

    float delta_time = GameClock::getInstance()->getCurrentTime() - start_time;

}

void FontSheet::renderToBMP(){
    std::string bmp_filename = "/tmp/" + filename + std::to_string(point) + ".bmp";
    GLubyte* image = new GLubyte[width*height];

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, image);

    int save_result = SOIL_save_image(bmp_filename.c_str(), SOIL_SAVE_TYPE_BMP, width, height, 1, image);
    if (!save_result){
        Debug::error("Error saving %s.\n", bmp_filename.c_str());
    }

    delete[] image;
    image = NULL;
}

Glyph FontSheet::getGlyph(char glyph_char){
    return character_map[glyph_char];
}

int FontSheet::getPointSize(){
    return point;
}

GLuint FontSheet::getTexture(){
    return texture_id;
}

unsigned int FontSheet::getWidth(){
    return width;
}

unsigned int FontSheet::getHeight(){
    return height;
}

double FontSheet::getMaxYBearing(){
    return max_y_bearing;
}
