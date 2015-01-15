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

    GLfloat getHeight(GLfloat, GLfloat);
    glm::vec3 getNormal(GLfloat, GLfloat);

private:

    void bindTextures();
    void updateUniformData();

    Mesh* generateMesh(Heightmap&);
    float getMapHeight(Heightmap&, int, int);

    TextureSet* texture_set;

    GLuint width;
    GLuint depth;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;

};

#endif
