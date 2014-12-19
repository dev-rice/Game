#ifndef CharacterMesh_h
#define CharacterMesh_h

#include "flat_mesh.h"

class CharacterMesh : public FlatMesh {
public:
    CharacterMesh();

    static const float CHARACTER_WIDTH;
    static const float CHARACTER_HEIGHT;
    static const float IMAGE_HEIGHT;
    static const float IMAGE_WIDTH;
};

#endif