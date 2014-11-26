#include "mesh.h"

Mesh::Mesh(const char* filename, GLfloat scale){
    this->scale = scale;

    MeshLoader mesh_loader = MeshLoader(filename);
    GLfloat* vertices = mesh_loader.getVertexArray();
    GLuint* elements = mesh_loader.getFaceArray();
    num_faces = mesh_loader.getFacesSize();
    
    // printf("\nVertices (size = %d):\n\t", mesh_loader.getVerticesSize() * sizeof(GLfloat));
    // for (int i = 0; i < mesh_loader.getVerticesSize(); ++i){
    //     if (i % 8 == 0 && i != 0){
    //         printf("\n\t");
    //     }
    //     printf("%f,\t", vertices[i]);
        
    // }
    // printf("\nElements (size = %d):\n\t", mesh_loader.getFacesSize() * sizeof(GLuint));
    // for (int i = 0; i < mesh_loader.getFacesSize(); ++i){
    //     if (i % 3 == 0 && i != 0){
    //         printf("\n\t");
    //     }
    //     printf("%d,\t", elements[i]);
        
    // }
    // printf("\n");

    GLuint vbo, ebo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);               // Generate 1 buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);  // Make vbo the active array buffer
    glBufferData(GL_ARRAY_BUFFER, mesh_loader.getVerticesSize() * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_loader.getFacesSize() * sizeof(GLuint), elements, GL_STATIC_DRAW);

}

void Mesh::draw(glm::mat4* view_matrix, glm::mat4* proj_matrix, glm::mat4* model_matrix){
    glBindVertexArray(vao);
    
    // Set the scale, this is not really going to be a thing, probably
    glUniform1f(glGetUniformLocation(shader_program, "scale"), this->scale);

    // Update the time uniform
    glUniform1f(glGetUniformLocation(shader_program, "time"), (float)glfwGetTime());

    // Update the light position
    GLfloat* light_array = new GLfloat[3];
    light_array[0] = 3.0f;
    light_array[1] = 2.5f * cos(glfwGetTime()) + 1.0f;
    light_array[2] = 2.0f * sin(glfwGetTime());

    glUniform3fv(glGetUniformLocation(shader_program, "light_position"), 1, light_array);

    light_array = NULL;
    delete light_array;

    // Update the model, view, and projection matrices
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, glm::value_ptr(*model_matrix));
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, glm::value_ptr(*view_matrix));    
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "proj"), 1, GL_FALSE, glm::value_ptr(*proj_matrix));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->diffuse_texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->specular_texture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, this->normal_map);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, this->emissive_texture);

    glDrawElements(GL_TRIANGLES, this->num_faces, GL_UNSIGNED_INT, 0);

}

void Mesh::attachShader(GLuint shader_program){
    this->shader_program = shader_program;

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

    glUniform1i(glGetUniformLocation(shader_program, "diffuse_texture"), 0);
    glUniform1i(glGetUniformLocation(shader_program, "specular_texture"), 1);
    glUniform1i(glGetUniformLocation(shader_program, "normal_map"), 2);
    glUniform1i(glGetUniformLocation(shader_program, "emissive_texture"), 3);
}

void Mesh::useTexture(const char* filename, GLuint filter, GLuint type){
        
    // Loading textures should probably be done at a higher level to prevent
    // having the same texture in multiple spots in memory.

    GLuint texture;
    glGenTextures(1, &texture);

    GLuint texture_value;
    switch(type){
        case DIFFUSE:
            texture_value = GL_TEXTURE0;
            this->diffuse_texture = texture;
            break;
        case SPECULAR:
            texture_value = GL_TEXTURE1;
            this->specular_texture = texture;
            break;
        case NORMAL:
            texture_value = GL_TEXTURE2;
            this->normal_map = texture;
            break;
        case EMISSIVE:
            texture_value = GL_TEXTURE3;
            this->emissive_texture = texture;
            break;
    }

    // Load the texture
    int width, height;
    unsigned char* image;
    // Set the active texture
    glActiveTexture(texture_value);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Load the image
    image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, image);
    // Set the texture wrapping to repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Do nearest interpolation for scaling the image up and down.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    // Mipmaps increase efficiency or something
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);

   
}