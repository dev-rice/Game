#include "drawable_placer.hpp"

DrawablePlacer::DrawablePlacer() : resource_loader() {
    resource_loader.setMeshPath("res/models/");
    resource_loader.setTexturePath("res/textures/");

    Mesh& mesh_ref = resource_loader.loadMesh("castle_tower.dae");
    current_drawable = new Doodad(&mesh_ref);
    current_drawable->setRotationEuler(M_PI / 2.0, 0, 0);
    current_drawable->setScale(3);

    Texture& diff_ref = resource_loader.loadTexture("castle_tower_diff.png");
    current_drawable->setDiffuse(diff_ref);
}

void DrawablePlacer::setDrawable(Drawable& drawable) {
    current_drawable = &drawable;
}

void DrawablePlacer::placeCurrentDrawable() {

}

void DrawablePlacer::update(glm::vec3 mouse_world_pos) {
    current_drawable->setPosition(mouse_world_pos);
}

Drawable& DrawablePlacer::getDrawable() {
    return *current_drawable;
}
