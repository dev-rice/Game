// Mesh:
//      Mesh handles all of the lowest level OpenGL drawing. It is a representation of a VAO.
//      From a top level perspective the Mesh holds geometry data: vertices and faces. This data
//      can either be passed in as std::vectors in the constructor or loaded from a .obj file. The
//      VAO is not complete without having an associated shader. Before using the mesh you must specify
//      which shader you will be using so that it knows the geometry data. Ultimately when using this object, 
//      you will get a VAO back. This VAO can then be bound such that you can attach uniform data and textures
//      to the shader at a higher level.

#include "mesh.h"

Mesh::Mesh(const char* filename){
    // This constructor loads geometry data (vertices and faces) from a .obj file.
    MeshLoader mesh_loader = MeshLoader(filename);
    std::vector<GLfloat> vertices = mesh_loader.getVertexArray();
    std::vector<GLuint>  elements = mesh_loader.getFaceArray();

    loadMeshData(vertices, elements);
}

Mesh::Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> elements){
    // This constructor loads geometry data (vertices and faces) from std::vectors.
    loadMeshData(vertices, elements);
}

void Mesh::loadMeshData(std::vector<GLfloat> vertices, std::vector<GLuint> elements){
    // The single vertex specification is:
    //      x, y, z, nx, ny, nz, u, v
    // Where:
    //      x = x position of vertex
    //      y = y position of vertex
    //      z = z position of vertex
    //      nx = x component of the normal vector
    //      ny = y component of the normal vector
    //      nz = z component of the normal vector
    //      u = U texture coordinate on UV map
    //      v = V texture coordinate on UV map

    // We need to know how many faces to draw later on.
    num_faces = elements.size();
    
    // Create our Vertex Array Object (VAO) which will hold our vertex and element data. 
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo, ebo;

    // Store all of the vertex data in a Vertex Buffer Object (VBO)
    glGenBuffers(1, &vbo);   
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    // Store all of the face data in a Element Buffer Object (EBO)
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size()* sizeof(GLuint), elements.data(), GL_STATIC_DRAW);

}

void Mesh::bindVAO(){
    // This will bind the vertex array for you so that you can use the mesh at a higher level.
    glBindVertexArray(vao);
}
void Mesh::draw(){
    // Draws the actual geometry. Textures and everything else are attached at a higher level.
    glDrawElements(GL_TRIANGLES, this->num_faces, GL_UNSIGNED_INT, 0);

}

void Mesh::attachGeometryToShader(GLuint shader_program){
    // As soon as you've bound a certain VAO, every time you call glVertexAttribPointer, 
    // that information will be stored in that VAO. This makes switching between different vertex data 
    // and vertex formats as easy as binding a different VAO.    

    // Because binding the geometry to the shader multiple times on the same vao causes problems, we 
    // check we have already bound the data.
    bool already_bound = std::find(bound_shaders.begin(), bound_shaders.end(), shader_program) != bound_shaders.end();

    // If this shader does not already have the data, bind it.
    if(!already_bound){
        glBindVertexArray(vao);

        // Get the reference to the "position" attribute defined in
        // the vertex shader
        GLint posAttrib = glGetAttribLocation(shader_program, "position");
        glEnableVertexAttribArray(posAttrib);
        // Load the position attributes from our vertex spec with width 8. The position
        // values start at index 0. Tell it to load 3 values.
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE,
                               8*sizeof(float), 0);

        GLint normalAttrib = glGetAttribLocation(shader_program, "normal");
        glEnableVertexAttribArray(normalAttrib);
        // Load the normal pointer from our vertex spec with width 8. The normal values
        // start at index 2. Tell it to load 3 values.
        glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE,
                               8*sizeof(float), (void*)(3*sizeof(float)));

        // Link the texture coordinates to the shader.
        GLint texAttrib = glGetAttribLocation(shader_program, "texcoord");
        glEnableVertexAttribArray(texAttrib);
        // Load the texture attributes from our vertex spec with width 8. The texture 
        // values start at index 8. Tell it to load 2 values.
        glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE,
                               8*sizeof(float), (void*)(6*sizeof(float)));

        // Keep track of which shaders we have already bound data to
        bound_shaders.push_back(shader_program);
    }
    
}