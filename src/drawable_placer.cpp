#include "drawable_placer.hpp"

DrawablePlacer::DrawablePlacer(Level& level) : level(&level) {
    ResourceLoader& resource_loader = this->level->getResourceLoader();
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

void DrawablePlacer::update(glm::vec3 mouse_world_pos) {
    current_drawable->setPosition(mouse_world_pos);
}

Drawable& DrawablePlacer::getDrawable() {
    return *current_drawable;
}

void DrawablePlacer::handleInput(SDL_Event event) {
    bool scroll_up = false;
    bool scroll_down = false;

    SDL_Scancode key_scancode = event.key.keysym.scancode;
    switch(event.type){
        case SDL_KEYDOWN:
            if (key_scancode == SDL_SCANCODE_UP){
                scroll_up = true;
            } else if (key_scancode == SDL_SCANCODE_DOWN) {
                scroll_down = true;
            }
        break;
        case SDL_MOUSEWHEEL:
            scroll_up = event.wheel.y > 0;
            scroll_down = event.wheel.y < 0;
        break;

        default:
        break;
    }

    float scale_diff = 0.05f;
    float current_scale = current_drawable->getScale();
    if (scroll_up) {
        current_drawable->setScale(current_scale + scale_diff);
    } else if (scroll_down) {
        current_drawable->setScale(current_scale - scale_diff);
    }
}
