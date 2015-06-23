#include "billboard_drawable.hpp"

BillboardDrawable::BillboardDrawable(ResourceLoader& resource_loader) {
    Mesh* plane_mesh = new PlaneMesh();
    Shader& shader_ref = resource_loader.loadShader("shaders/billboard.vs", "shaders/billboard.fs");

    load(*plane_mesh, shader_ref, glm::vec3(0, 0, 0), 1.0);

    Texture& billboard_texture = resource_loader.loadTexture("fire_part.png");
    setEmissive(billboard_texture);
}

BillboardDrawable::~BillboardDrawable(){
    delete mesh;
    mesh = NULL;
}

Drawable* BillboardDrawable::clone() {
    return new BillboardDrawable(*this);
}

void BillboardDrawable::draw() {
    // // Disable depth sorting when rendering
    // // particles.
    // glDepthFunc(GL_LEQUAL);
    // glDepthMask(GL_FALSE);

    Drawable::draw();

    // // Re enable depth sorting for everything else
    // // (really should not be here)
    // glDepthFunc(GL_LESS);
    // glDepthMask(GL_TRUE);
}

void BillboardDrawable::updateUniformData(){
    // Set the scale, this is not really going to be a thing, probably
    // ^ It's definitely a thing
    glUniform1f(glGetUniformLocation(shader->getGLId(), "scale"), scale);

    // Set the opacity in the shader
    glUniform1f(glGetUniformLocation(shader->getGLId(), "opacity"), (float)opacity);

    // Set the planar rotation
    glUniform1f(glGetUniformLocation(shader->getGLId(), "plane_rotation"), (float)plane_rotation);

}

string BillboardDrawable::asJsonString() {
    string json_string = "";
    return json_string;
}
