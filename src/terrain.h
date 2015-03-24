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
#include "layered_textures.h"
#include "texture_layer.h"
#include "texture_painter.h"

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

    void addSplatmap(GLuint splat);
    void addDiffuse(GLuint diff, GLuint splat, int layer_num, char channel);
    void fillSplatmaps();

    void printPathing();

    bool isOnTerrain(GLfloat, GLfloat, GLfloat);

    bool canPath(int, int);

    void paintSplatmap(glm::vec3 position);
    void eraseSplatmap(glm::vec3 position);
    TextureLayer getCurrentLayer();
    void setPaintLayer(GLuint layer);

    std::string saveData(std::string name);

    LayeredTextures* getLayeredTextures();
    TexturePainter* getTexturePainter();

private:

    void updateUniformData();

    GLubyte* renderHeightmapAsImage();

    void initializeBaseMesh(Heightmap&);
    Mesh* generateMesh(Heightmap&);
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
    float amplification;

    LayeredTextures* layered_textures;

    TexturePainter* texture_painter;

};

#endif
