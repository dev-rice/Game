#ifndef TerrainMesh_h
#define TerrainMesh_h

#include "mesh.h"

class TerrainMesh : public Mesh {
public:
    TerrainMesh(std::vector<TerrainVertex>, std::vector<GLuint>);

    void attachGeometryToShader(GLuint shader_program);

private:

};

#endif
