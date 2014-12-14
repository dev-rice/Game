// Drawable:
//      The Drawable class is a parent class for any object that needs to be rendered in our 3D world.
//      It has a position, rotation (local), a shader, and a pointer to a Mesh (geometry data).

#include "drawable.h"

Drawable::Drawable(Mesh* mesh, GLuint shader_program){
    load(mesh, shader_program, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
}

Drawable::Drawable(Mesh* mesh, GLuint shader_program, glm::vec3 position, GLfloat scale) {
    load(mesh, shader_program, position, glm::vec3(0.0f, 0.0f, 0.0f), scale);
}

Drawable::Drawable(Mesh* mesh, GLuint shader_program, glm::vec3 position, glm::vec3 rotation, GLfloat scale) {
    load(mesh, shader_program, position, rotation, scale);

}

void Drawable::load(Mesh* mesh, GLuint shader_program, glm::vec3 position, glm::vec3 rotation, GLfloat scale) {
    // Set the position, rotation, scale, and mesh pointer
    this->position = position;
    this->rotation = rotation;
    this->mesh = mesh;
    this->scale = scale;
    
    // Set the shader program and load the geometry data
    // from the mesh onto it.
    this->shader_program = shader_program;
    this->mesh->attachGeometryToShader(shader_program);

}

void Drawable::draw(Camera* camera, glm::mat4* proj_matrix){
    glUseProgram(shader_program);
    
    // Bind the Mesh's VAO. This lets us put transformations and textures on
    // top of the geometry. 
    mesh->bindVAO();

    // Get the current view matrix from the camera.
    glm::mat4 view_matrix = camera->getViewMatrix();

    // We need to update the model matrix to account for any rotations
    // or translations that have occured since the last draw call.
    updateModelMatrix();    

    // Update basic shader uniform data
    updateUniformData(&view_matrix, proj_matrix);
    
    // Make sure to use this Drawable's textures  
    bindTextures();

    // Draw the geometry
    mesh->draw();
}

void Drawable::attachTextureSet(TextureSet texture_set){
    // Prepare the loacations for textures to load into and give this drawable some textures. These are specific 
    // to Drawable for now but will later be moved down to the child classes such that they can specify different
    // amounts and types of textures to use.

    // When we set this uniform we tell the shader that "diffuse_texture" will be loaded from the 0th texture, and so on. 
    // The actual images these numbers point to are specified later in bindTextures().
    glUniform1i(glGetUniformLocation(shader_program, "diffuse_texture"), 0);
    glUniform1i(glGetUniformLocation(shader_program, "specular_texture"), 1);
    glUniform1i(glGetUniformLocation(shader_program, "normal_map"), 2);
    glUniform1i(glGetUniformLocation(shader_program, "emissive_texture"), 3);

    this->texture_set = new TextureSet(texture_set);
}

void Drawable::rotateAround(glm::vec3 rotation_axis, GLfloat angle){
    // glm::mat4 rotation_matrix = glm::mat4();
    // rotation_matrix = glm::rotate
}

void Drawable::bindTextures(){
    // Put each texture into the correct location for this Drawable. GL_TEXTURE0-3
    // correspond to the uniforms set in attachTextureSet(). This is where we actually 
    // tell the graphics card which textures to use.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_set->diffuse);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_set->specular);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture_set->normal);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, texture_set->emissive);
}

void Drawable::updateUniformData(glm::mat4* view_matrix, glm::mat4* proj_matrix){
    // Set the scale, this is not really going to be a thing, probably
    // ^ It's definitely a thing
    glUniform1f(glGetUniformLocation(shader_program, "scale"), scale);

    // Tell the shader the current time
    glUniform1f(glGetUniformLocation(shader_program, "time"), (float)glfwGetTime());

    // Update the current model, view, and projection matrices in the shader.
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, glm::value_ptr(model_matrix));
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, glm::value_ptr(*view_matrix));    
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "proj"), 1, GL_FALSE, glm::value_ptr(*proj_matrix));
}

void Drawable::updateModelMatrix(){
    // Creates the model matrix from the Drawables position and rotation.
    glm::mat4 translation_matrix = glm::translate(glm::mat4(), position);

    // Axes on which to preform the rotations.
    glm::vec3 x_axis = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 y_axis = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 z_axis = glm::vec3(0.0f, 0.0f, 1.0f);

    // Rotate the model about each axis.
    glm::mat4 rotation_matrix;
    rotation_matrix = glm::rotate(rotation_matrix, rotation.x, x_axis);
    rotation_matrix = glm::rotate(rotation_matrix, rotation.y, y_axis);
    rotation_matrix = glm::rotate(rotation_matrix, rotation.z, z_axis);

    model_matrix = translation_matrix * rotation_matrix;
}

