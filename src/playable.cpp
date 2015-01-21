// playable.cpp
// Trevor Westphal

#include "playable.h"

Playable::Playable() : Drawable(){

}

Playable::Playable(Mesh* mesh, GLuint shader_program, glm::vec3 position, GLfloat scale) : Drawable(mesh, shader_program, position, scale) {
	
}

void Playable::updateUniformData(){
	 glUniform1f(glGetUniformLocation(shader_program, "scale"), scale);
}