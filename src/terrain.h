#ifndef Terrain_h
#define Terrain_h

#include <SOIL.h>
#include <string>
#include <vector>

#include "mesh.h"
#include "drawable.h"

class Terrain : public Drawable {
public:
    Terrain (GLuint, std::string);

    void attachTextureSet(TextureSet*);

    int getDepth();
    int getWidth();

private:

    void bindTextures();
    void updateUniformData();

    Mesh* generateMesh();
    float getHeight(int, int);

    TextureSet* texture_set;
    unsigned char* image;
    int image_width, image_height;

};

#endif
