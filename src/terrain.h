#ifndef Terrain_h
#define Terrain_h

#include <SOIL.h>
#include <string>
#include <vector>

#include "mesh.h"
#include "drawable.h"

struct Heightmap {
    unsigned char* image;
    int width;
    int height;
};

class Terrain : public Drawable {
public:
    Terrain (GLuint, std::string);

    void attachTextureSet(TextureSet*);

    int getDepth();
    int getWidth();

private:

    void bindTextures();
    void updateUniformData();

    Mesh* generateMesh(Heightmap&);
    float getHeight(Heightmap&, int, int);

    TextureSet* texture_set;

};

#endif
