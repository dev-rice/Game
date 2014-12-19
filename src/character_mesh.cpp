#include "character_mesh.h"

const float CharacterMesh::CHARACTER_WIDTH = 16;
const float CharacterMesh::CHARACTER_HEIGHT = 20;
const float CharacterMesh::IMAGE_HEIGHT = 256;
const float CharacterMesh::IMAGE_WIDTH = 256;

CharacterMesh::CharacterMesh(){
    // Create 2D mesh for framebuffer to draw onto.
    float delta_v = CHARACTER_HEIGHT / IMAGE_HEIGHT;
    float delta_u = CHARACTER_WIDTH / IMAGE_WIDTH;

    // GLfloat planeVerts[] = {
    //          -1.0f,  1.0f,  0.0f, delta_v,
    //           1.0f,  1.0f,  delta_u, delta_v,
    //           1.0f, -1.0f,  delta_u, 0.0f,

    //           1.0f, -1.0f,  delta_u, 0.0f,
    //          -1.0f, -1.0f,  0.0f, 0.0f,
    //          -1.0f,  1.0f,  0.0f, delta_v,
    // };
    GLfloat planeVerts[] = {
             -1.0f,  1.0f,  0.0f, 0.0f,
              1.0f,  1.0f,  delta_u, 0.0f,
              1.0f, -1.0f,  delta_u, delta_v,

              1.0f, -1.0f,  delta_u, delta_v,
             -1.0f, -1.0f,  0.0f, delta_v,
             -1.0f,  1.0f,  0.0f, 0.0f,
    };

    std::vector<GLfloat> vertices(planeVerts, planeVerts + sizeof(planeVerts) / sizeof(GLfloat));
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);   
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);    
}  
