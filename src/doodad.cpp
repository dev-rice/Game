#include "doodad.h"

Doodad::Doodad(Mesh* mesh, GLuint shader_program) : Drawable(mesh, shader_program){

}

Doodad::Doodad(Mesh* mesh, GLuint shader_program, glm::vec3 position, GLfloat scale): Drawable(mesh, shader_program, position, scale) {

}

Doodad::Doodad(Mesh* mesh, GLuint shader_program, glm::vec3 position, glm::vec3 rotation, GLfloat scale): Drawable(mesh, shader_program, position, rotation, scale) {

}

void Doodad::attachTextureSet(TextureSet* texture_set){
    // Prepare the loacations for textures to load into and give this drawable some textures. These are specific
    // to Drawable for now but will later be moved down to the child classes such that they can specify different
    // amounts and types of textures to use.

    // When we set this uniform we tell the shader that "diffuse_texture" will be loaded from the 0th texture, and so on.
    // The actual images these numbers point to are specified later in bindTextures().

    this->texture_set = texture_set;
}

void Doodad::updateUniformData(){
    // Set the scale, this is not really going to be a thing, probably
    // ^ It's definitely a thing
    glUniform1f(glGetUniformLocation(shader_program, "scale"), scale);

    // Tell the shader the current time
    // glUniform1f(glGetUniformLocation(shader_program, "time"), (float)glfwGetTime());

}
