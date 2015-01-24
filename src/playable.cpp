// playable.cpp
// Trevor Westphal

#include "playable.h"

Playable::Playable() : Drawable(){

}

Playable::Playable(Mesh* mesh, GLuint shader_program, glm::vec3 position, GLfloat scale) : Drawable(mesh, shader_program, position, scale) {

	Mesh* selection_ring_mesh = new Mesh("res/models/selection_ring.obj");

	selection_ring = new Doodad(selection_ring_mesh, shader_program, position, 1.0f);
	selection_ring->setEmissive(TextureLoader::loadTextureFromFile("res/textures/selection_ring.png", GL_LINEAR));

	selected = false;
}

void Playable::updateUniformData(){
	glUniform1f(glGetUniformLocation(shader_program, "scale"), scale);
}

void Playable::update(Terrain* ground){
	position.y = ground->getHeight(position.x, position.z);
	position.x += 0.05f;

	selection_ring->setPosition(glm::vec3(position.x, position.y + 0.5, position.z));
}

void Playable::draw(){
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    if(selected){
    	selection_ring->draw();
    }

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

void Playable::select(){
    selected = true;
}