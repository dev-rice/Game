#ifndef Terrain_h
#define Terrain_h

#include "drawable.h"

class Terrain : public Drawable {
public:
    Terrain () {;}
    Terrain(Mesh*, GLuint);
    Terrain(Mesh*, GLuint, glm::vec3, GLfloat);
    Terrain(Mesh*, GLuint, glm::vec3, glm::vec3, GLfloat);

    void attachTextureSet(TextureSet);

private:
    void bindTextures();
    void updateUniformData();

    TextureSet* texture_set;
};

#endif
