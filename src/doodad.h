#ifndef Doodad_h
#define Doodad_h

#include "drawable.h"

class Doodad: public Drawable {
public:
    Doodad(Mesh*);
    Doodad(Mesh*, GLuint);
    Doodad(Mesh*, GLuint, glm::vec3, GLfloat);

private:
    void updateUniformData();

};

#endif
