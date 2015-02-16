#include "terrain_mesh.h"

TerrainMesh::TerrainMesh(std::vector<TerrainVertex> vertices, std::vector<GLuint> elements){
    // This constructor loads geometry data (vertices and faces) from std::vectors.
    std::vector<GLfloat> out_vertices;
    for (int i = 0; i < vertices.size(); ++i){
        TerrainVertex vertex = vertices[i];
        out_vertices.push_back(vertex.position.x);
        out_vertices.push_back(vertex.position.y);
        out_vertices.push_back(vertex.position.z);
        out_vertices.push_back(vertex.normal.x);
        out_vertices.push_back(vertex.normal.y);
        out_vertices.push_back(vertex.normal.z);
        out_vertices.push_back(vertex.tangent.x);
        out_vertices.push_back(vertex.tangent.y);
        out_vertices.push_back(vertex.tangent.z);
        out_vertices.push_back(vertex.binormal.x);
        out_vertices.push_back(vertex.binormal.y);
        out_vertices.push_back(vertex.binormal.z);
        out_vertices.push_back(vertex.texcoord.x);
        out_vertices.push_back(vertex.texcoord.y);
    }

    loadMeshData(out_vertices, elements);
}
