#include "doodad.h"

Doodad::Doodad(Mesh* mesh, GLuint shader_program) : Drawable(mesh, shader_program){

}

Doodad::Doodad(Mesh* mesh, GLuint shader_program, glm::vec3 position, GLfloat scale): Drawable(mesh, shader_program, position, scale) {

}

Doodad::Doodad(Mesh* mesh, GLuint shader_program, glm::vec3 position, glm::vec3 rotation, GLfloat scale): Drawable(mesh, shader_program, position, rotation, scale) {

}

void Doodad::bindTextures(){
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