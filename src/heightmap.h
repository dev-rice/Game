#ifndef Heightmap_h
#define Heightmap_h

#include "includes/gl.hpp"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

#include "debug.h"

#include "texture.hpp"

class Heightmap {
public:
    // Number of components in the map image, 4 for RGBA
    const int COMPONENTS = 4;

    Heightmap(std::string filename, float ampliciation);
    ~Heightmap();

    float getMapHeight(int x, int y);

    void updateImage();
    unsigned char* getImage();

    int getWidth() {return width;}
    int getHeight() {return height;}

    Texture getTexture();
private:
    unsigned char* image;
    int width;
    int height;
    float amplification;
    Texture texture;
};

#endif
