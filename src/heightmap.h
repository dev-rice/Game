#ifndef Heightmap_h
#define Heightmap_h

#include <SOIL.h>
#include <string>

#include "debug.h"

class Heightmap {
public:
    // Number of components in the map image, 4 for RGBA
    const int COMPONENTS = 4;

    Heightmap(std::string filename, float ampliciation);
    ~Heightmap();

    float getMapHeight(int x, int y);

    int getWidth() {return width;}
    int getHeight() {return height;}
private:
    unsigned char* image;
    int width;
    int height;
    float amplification;
};

#endif
