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
    setShader(shader_program);

}

void Drawable::setShader(GLuint shader_program){
    this->shader_program = shader_program;
    this->mesh->attachGeometryToShader(shader_program);

    GLint global_matrix_location = glGetUniformBlockIndex(shader_program, "GlobalMatrices");
    // Debug::info("Matrix location is %d on %s.\n", global_matrix_location,
        // ShaderLoader::getShaderName(shader_program).c_str());
    glUniformBlockBinding(shader_program, global_matrix_location, 1);
}

void Drawable::setupShadows(GLuint shadow_map, glm::mat4 depth_view, glm::mat4 depth_proj){
    this->shadow_map = shadow_map;
    this->depth_view = depth_view;
    this->depth_proj = depth_proj;
}

void Drawable::draw(){
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glUseProgram(shader_program);

    // Bind the Mesh's VAO. This lets us put transformations and textures on
    // top of the geometry.
    mesh->bindVAO();

    // We need to update the model matrix to account for any rotations
    // or translations that have occured since the last draw call.
    updateModelMatrix();

    // Update the current model, view, and projection matrices in the shader. These are standard for all
    // Drawables so they should always be updated in draw. Child specific data is updated in updateUniformData().
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, glm::value_ptr(model_matrix));

    // Update other shader data
    updateUniformData();

    // Make sure to use this Drawable's textures
    bindTextures();

    // Draw the geometry
    mesh->draw();
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
