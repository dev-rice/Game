#ifndef CharacterMesh_h
#define CharacterMesh_h

#include "flat_mesh.h"
#include "font_sheet.h"

class CharacterMesh : public FlatMesh {
public:

    static CharacterMesh* getInstance();

private:

    CharacterMesh();
    static CharacterMesh* instance;
};

#endif
