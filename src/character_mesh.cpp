#include "character_mesh.h"

CharacterMesh::CharacterMesh(){
    // Create 2D mesh for framebuffer to draw onto.
    float delta_v = CHARACTER_HEIGHT / IMAGE_HEIGHT;
    float delta_u = CHARACTER_WIDTH / IMAGE_WIDTH;

    GLfloat planeVerts[] = {
             -1.0f,  1.0f,  0.0f, delta_v,
              1.0f,  1.0f,  delta_u, delta_v,
              1.0f, -1.0f,  delta_u, 0.0f,

              1.0f, -1.0f,  delta_u, 0.0f,
             -1.0f, -1.0f,  0.0f, 0.0f,
             -1.0f,  1.0f,  0.0f, delta_v,
    };

    std::vector<GLfloat> vertices(planeVerts, planeVerts + sizeof(planeVerts) / sizeof(GLfloat));
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);   
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);    
}  
