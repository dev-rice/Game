#include "mesh.h"

Mesh::Mesh(const char* filename){

    MeshLoader mesh_loader = MeshLoader(filename);
    std::vector<GLfloat> vertices = mesh_loader.getVertexArray();
    std::vector<GLuint>  elements = mesh_loader.getFaceArray();

    loadMeshData(vertices, elements);
}

Mesh::Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> elements){

    loadMeshData(vertices, elements);
}

void Mesh::loadMeshData(std::vector<GLfloat> vertices, std::vector<GLuint> elements){
    num_faces = elements.size();
    
    GLuint vbo, ebo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);               // Generate 1 buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);  // Make vbo the active array buffer
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size()* sizeof(GLuint), elements.data(), GL_STATIC_DRAW);

}

void Mesh::bindVAO(){
    glBindVertexArray(vao);
}
void Mesh::draw(){
    
    glDrawElements(GL_TRIANGLES, this->num_faces, GL_UNSIGNED_INT, 0);

}

void Mesh::attachGeometryToShader(GLuint shader_program){
    // As soon as you've bound a certain VAO, every time you call glVertexAttribPointer, 
    //that information will be stored in that VAO. This makes switching between different vertex data 
    //and vertex formats as easy as binding a different VAO! Just remember that a VAO doesn't store 
    //any vertex data by itself, it just references the VBOs you've created and how to retrieve the
    // attribute values from them.

    #warning For some reason we do not need to bind the VAO before attaching the geometry data to the shader.
    // glBindVertexArray(vao);


    // Get the reference to the "position" attribute defined in
    // the vertex shader
    GLint posAttrib = glGetAttribLocation(shader_program, "position");
    glEnableVertexAttribArray(posAttrib);
    // Load the position attributes from our array with width 3. The position
    // values start at index 0. Tell it to load 2 values
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE,
                           8*sizeof(float), 0);

    GLint normalAttrib = glGetAttribLocation(shader_program, "normal");
    glEnableVertexAttribArray(normalAttrib);
    // Load the normal pointer from our array with width 3. The normal values
    // start at index 2. Tell it to load 3 value
    glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE,
                           8*sizeof(float), (void*)(3*sizeof(float)));

    // Link the texture coordinates to the shader.
    GLint texAttrib = glGetAttribLocation(shader_program, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE,
                           8*sizeof(float), (void*)(6*sizeof(float)));

    
}