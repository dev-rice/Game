// map.h
// Trevor Westphal

#include "lodepng.h"

class Map {
public:
    Map(const char *, const char**);
private:
    void generateMesh();
    void draw();
};