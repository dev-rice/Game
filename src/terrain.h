#ifndef Terrain_h
#define Terrain_h

#include <SOIL.h>
#include <string>
#include <vector>

#include "mesh.h"
#include "drawable.h"

class Terrain : public Drawable {
public:
    enum class VertexType { CORNER, X_EDGE, Z_EDGE, INTERNAL};

    Terrain (GLuint, std::string);
    // Terrain(Mesh*, GLuint);
    // Terrain(Mesh*, GLuint, glm::vec3, GLfloat);
    // Terrain(Mesh*, GLuint, glm::vec3, GLfloat, const char*);
    // Terrain(Mesh*, GLuint, glm::vec3, glm::vec3, GLfloat);

    void attachTextureSet(TextureSet);

private:

    void bindTextures();
    void updateUniformData();

    Mesh* generateMesh();
    glm::vec3 getVertexPosition(GLuint, std::vector<GLfloat>*);
    Terrain::VertexType getVertexType(glm::vec3);
    float getHeight(int, int);

    TextureSet* texture_set;
    unsigned char* image;
    int image_width, image_height;


};

#endif
