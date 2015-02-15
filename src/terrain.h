#ifndef Terrain_h
#define Terrain_h

#include <SOIL.h>
#include <string>
#include <vector>
#include <cmath>
#include <unordered_map>

#include "mesh.h"
#include "drawable.h"
#include "heightmap.h"


class Terrain : public Drawable {
public:
    Terrain (GLuint s, std::string h) : Terrain(s, h, 10.0f) {;}
    Terrain (GLuint, std::string, float);

    int getDepth() {return depth;}
    int getWidth() {return width;}

    GLfloat getHeight(GLfloat, GLfloat);
    GLfloat getHeightInterpolated(GLfloat, GLfloat);
    glm::vec3 getNormal(GLfloat, GLfloat);
    float getSteepness(GLfloat, GLfloat);
    void printPathing();

    bool isOnTerrain(GLfloat, GLfloat, GLfloat);

    bool canPath(int, int);

private:

    void updateUniformData();

    void initializeBaseMesh(Heightmap&);
    Mesh* generateMesh(std::string filename, float);
    int getIndex(int x, int y);
    int getIndex(int x, int y, int width);

    bool** pathing_array;

    std::vector<Vertex> vertices;

    int width;
    int depth;
    int start_x;
    int start_z;


};

#endif
