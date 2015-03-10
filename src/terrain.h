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
#include "vertex.h"
#include "terrain_mesh.h"
#include "game_clock.h"

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
    float getMaxHeight(){return max_height;}

    void setSplatmap(GLuint splat, int index);
    void setDiffuse(GLuint diff, int index, char channel);

    void printPathing();

    bool isOnTerrain(GLfloat, GLfloat, GLfloat);

    bool canPath(int, int);

    void paintSplatmap(glm::vec3);

private:

    void updateUniformData();

    void initializeBaseMesh(Heightmap&);
    Mesh* generateMesh(std::string filename, float);
    int getIndex(int x, int y);
    int getIndex(int x, int y, int width);

    virtual void bindTextures();
    virtual void setTextureLocations();

    bool** pathing_array;

    std::vector<TerrainVertex> vertices;

    int width;
    int depth;
    int start_x;
    int start_z;
    float max_height;

    GLuint splatmaps[2];
    GLuint diffuse_textures[7];
    GLuint channels[7];

    GLuint splatmap_painted;
    GLuint diffuse_painted;

    GLubyte brush[16];


};

#endif
