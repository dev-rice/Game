#include "heightmap.h"

// From http://www.exploringbinary.com/ten-ways-to-check-if-an-integer-is-a-power-of-two-in-c/
int isPowerOfTwo (unsigned int x){
    return ((x != 0) && ((x & (~x + 1)) == x));
}

Heightmap::Heightmap(std::string filename, float amplification){
    this->amplification = amplification;
    image = SOIL_load_image(filename.c_str(), &(width), &(height),
        0, SOIL_LOAD_RGBA);

    if(image){
        Debug::info("Loaded heightmap \"%s\" (%d by %d) into memory.\n",
            filename.c_str(), width, height );
    } else {
        Debug::error("Could not load heightmap \"%s\" into memory.\n",
            filename.c_str());
    }

    if(!isPowerOfTwo(width) || !isPowerOfTwo(height)){
        Debug::warning("Terrain map size is not base 2."
            " Mesh generation may behave incorrectly.\n");
    }
}

Heightmap::~Heightmap(){
    SOIL_free_image_data(image);
}


float Heightmap::getMapHeight(int x, int y){
    // Scaling factor for the height map data
    int red = image[(y * width + x)*4 + 0];
    int green = image[(y * width + x)*4 + 1];
    int blue = image[(y * width + x)*4 + 2];

    // Scale the height such that the value is between 0.0 and 1.0
    float map_height = float(red + green + blue) / (3.0f * 255.0);
    map_height *= amplification;
    return map_height;
}
