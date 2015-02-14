#ifndef Heightmap_h
#define Heightmap_h

#include <SOIL.h>
#include <string>

#include "debug.h"

class Heightmap {
public:
    Heightmap(std::string filename);
    ~Heightmap();

    float getMapHeight(int x, int y);

    int getWidth() {return width;}
    int getHeight() {return height;}
private:
    unsigned char* image;
    int width;
    int height;
};

#endif
