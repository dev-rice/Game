#include "font_sheet.h"

FontSheet::FontSheet(std::string filename, int pixel_size) {
    this->filename = filename;
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
    } else if (error) {
        Debug::error("Error loading font face '%s'.\n", font_filename.c_str());
    }

    Debug::info("Face data:\n");
    Debug::info("  num_glyphs == %d\n", face->num_glyphs);
    Debug::info("  num_fixed_sizes == %d\n", face->num_fixed_sizes);

    int point_size = 64;
    error = FT_Set_Pixel_Sizes(face, 0, point_size);
    if (error){
        Debug::error("Cannot set face size to %d", point_size);
    }

    width = 0;
    height = 0;

    for (int i = 0; i < 128; ++i){
        char to_render = i;

        int load_flags = FT_LOAD_RENDER;
        error = FT_Load_Char(face, to_render, load_flags);
        if (error){
            Debug::error("Cannot load glyph %c.\n", to_render);
        }

        FT_GlyphSlot glyph = face->glyph;
        width += point_size;
        height = point_size;
    }

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    float start_time = GameClock::getInstance()->getCurrentTime();

    int x_offset = 0;
    int y_offset = 0;
    for (int i = 0; i < 128; ++i){
        char to_render = i;

        int load_flags = FT_LOAD_RENDER;
        error = FT_Load_Char(face, to_render, load_flags);
        if (error){
            Debug::error("Cannot load glyph %d.\n", to_render);
        }

        FT_GlyphSlot glyph = face->glyph;

        x_offset += point_size;

        glTexSubImage2D(GL_TEXTURE_2D, 0, x_offset, 0, glyph->bitmap.width, glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);
    }

    float delta_time = GameClock::getInstance()->getCurrentTime() - start_time;
    Debug::info("It took %.6f seconds to generate the font sheet for %s.\n", delta_time, font_filename.c_str());

    // Set the texture wrapping to repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Do nearest interpolation for scaling the image up and down.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void FontSheet::renderToBMP(){
    std::string bmp_filename = "font_render/" + filename + ".bmp";
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

GLuint FontSheet::getTexture(){
    return texture_id;
}

unsigned int FontSheet::getWidth(){
    return width;
}

unsigned int FontSheet::getHeight(){
    return height;
}
