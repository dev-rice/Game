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
    glUniform1i(glGetUniformLocation(shader_program, "diffuse_texture"), 0);
    glUniform1i(glGetUniformLocation(shader_program, "specular_texture"), 1);
    glUniform1i(glGetUniformLocation(shader_program, "normal_map"), 2);
    glUniform1i(glGetUniformLocation(shader_program, "emissive_texture"), 3);
    glUniform1i(glGetUniformLocation(shader_program, "shadow_map"), 4);

    this->texture_set = texture_set;
}

void Doodad::bindTextures(){
    // Put each texture into the correct location for this Drawable. GL_TEXTURE0-3
    // correspond to the uniforms set in attachTextureSet(). This is where we actually
    // tell the graphics card which textures to use.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_set->getDiffuse());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_set->getSpecular());

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture_set->getNormal());

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, texture_set->getEmissive());

}

void Doodad::updateUniformData(){
    // Set the scale, this is not really going to be a thing, probably
    // ^ It's definitely a thing
    glUniform1f(glGetUniformLocation(shader_program, "scale"), scale);

    // Tell the shader the current time
    // glUniform1f(glGetUniformLocation(shader_program, "time"), (float)glfwGetTime());

    glUniformMatrix4fv(glGetUniformLocation(shader_program, "depth_view"), 1, GL_FALSE, glm::value_ptr(depth_view));
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "depth_proj"), 1, GL_FALSE, glm::value_ptr(depth_proj));

}
