#include "doodad.h"

Doodad::Doodad(Mesh* mesh, GLuint shader_program) :
    Drawable(mesh, shader_program){

}

Doodad::Doodad(Mesh* mesh, GLuint shader_program, glm::vec3 position, GLfloat scale):
    Drawable(mesh, shader_program, position, scale) {

}

Doodad::Doodad(Mesh* mesh, GLuint shader_program, glm::vec3 position, glm::vec3 rotation, GLfloat scale):
    Drawable(mesh, shader_program, position, rotation, scale) {

}

void Doodad::updateUniformData(){
    // Set the scale, this is not really going to be a thing, probably
    // ^ It's definitely a thing
    glUniform1f(glGetUniformLocation(shader_program, "scale"), scale);

}
