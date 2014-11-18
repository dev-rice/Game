#include "model.h"

Model::Model(const char* filename, GLfloat scale){
    Mesh mesh = Mesh(filename);
    GLfloat* vertices = mesh.getVertexArray();
    GLuint* elements = mesh.getFaceArray();

    num_faces = mesh.getFacesSize();
    // printf("\nVertices (size = %d):\n\t", mesh.getVerticesSize() * sizeof(GLfloat));
    // for (int i = 0; i < mesh.getVerticesSize(); ++i){
    //     if (i % 8 == 0 && i != 0){
    //         printf("\n\t");
    //     }
    //     printf("%f,\t", vertices[i]);
        
    // }
    // printf("\nElements (size = %d):\n\t", mesh.getFacesSize() * sizeof(GLuint));
    // for (int i = 0; i < mesh.getFacesSize(); ++i){
    //     if (i % 3 == 0 && i != 0){
    //         printf("\n\t");
    //     }
    //     printf("%d,\t", elements[i]);
        
    // }
    // printf("\n");
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);               // Generate 1 buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);  // Make vbo the active array buffer
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, mesh.getVerticesSize() * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.getFacesSize() * sizeof(GLuint), elements, GL_STATIC_DRAW);

    glGenTextures(1, &texture);

    this->scale = scale;

}

void Model::draw(glm::mat4* view_matrix, glm::mat4* proj_matrix, glm::mat4* model_matrix){
    glBindVertexArray(vao);
    
    // Tell the shader which texture to use
    glUniform1i(glGetUniformLocation(shader_program, "tex"), texture_number);
    
    // Set the scale, this is not really going to be a thing
    glUniform1f(glGetUniformLocation(shader_program, "scale"), this->scale);

    // Update the time uniform
    glUniform1f(glGetUniformLocation(shader_program, "time"), (float)glfwGetTime());

    // Update the model, view, and projection matrices
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, glm::value_ptr(*model_matrix));
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, glm::value_ptr(*view_matrix));    
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "proj"), 1, GL_FALSE, glm::value_ptr(*proj_matrix));

    // Draw the things
    glDrawElements(GL_TRIANGLES, num_faces, GL_UNSIGNED_INT, 0);

}

void Model::attachShader(GLuint shader_program){
    this->shader_program = shader_program;

    // Get the reference to the "position" attribute defined in
    // the vertex shader
    GLint posAttrib = glGetAttribLocation(shader_program, "position");
    glEnableVertexAttribArray(posAttrib);
    // Load the position attributes from our array with width 3. The position
    // values start at index 0. Tell it to load 2 values
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE,
                           8*sizeof(float), 0);

    GLint colAttrib = glGetAttribLocation(shader_program, "color");
    glEnableVertexAttribArray(colAttrib);
    // Load the color pointer from our array with width 3. The color values
    // start at index 2. Tell it to load 3 value
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
                           8*sizeof(float), (void*)(3*sizeof(float)));

    // Link the texture coordinates to the shader.
    GLint texAttrib = glGetAttribLocation(shader_program, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE,
                           8*sizeof(float), (void*)(6*sizeof(float)));
}

void Model::useTexture(const char* filename, GLuint texture_value){
    // This is a bade way to do it, no idea why but it slows down
    // a ton when using two textures.

    // Load the texture
    int width, height;
    unsigned char* image;
    // Set the active texture
    glActiveTexture(texture_value);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Load the image
    image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                  GL_UNSIGNED_BYTE, image);
    // Set the texture wrapping to repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Do nearest interpolation for scaling the image up and down.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Mipmaps increase efficiency or something
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    // Link the texture

    // No idea how to avoid this right now but the texture
    // numbers go like GL_TEXTURE0 = 33984, GL_TEXTURE1 = 33985, ...
    GLuint texture_number = texture_value - 33984;
    this->texture_number = texture_number;
}