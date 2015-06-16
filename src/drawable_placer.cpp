#include "drawable_placer.hpp"

DrawablePlacer::DrawablePlacer(Level& level) : level(&level) {
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
    bool scale_up = false;
    bool scale_down = false;
    bool place_doodad = false;

    SDL_Scancode key_scancode = event.key.keysym.scancode;
    switch(event.type){
        case SDL_KEYDOWN:
            if (key_scancode == SDL_SCANCODE_UP){
                scale_up = true;
            } else if (key_scancode == SDL_SCANCODE_DOWN) {
                scale_down = true;
            }
        break;
        case SDL_MOUSEWHEEL:
            scale_up = event.wheel.y > 0;
            scale_down = event.wheel.y < 0;
        break;

        case SDL_MOUSEBUTTONDOWN:
            place_doodad = event.button.button == SDL_BUTTON_LEFT;
        break;

        default:
        break;
    }

    float scale_diff = 0.1f;
    float current_scale = current_drawable->getScale();
    if (scale_up) {
        current_drawable->setScale(current_scale + scale_diff);
    } else if (scale_down) {
        current_drawable->setScale(current_scale - scale_diff);
    }

    if (place_doodad) {
        this->level->getGameMap().placeTempDrawable();
        initializeNewDoodad();
    }
}

void DrawablePlacer::initializeNewDoodad() {
    current_drawable = createDefaultDoodad();
    this->level->getGameMap().setTempDrawable(*current_drawable);
}

void DrawablePlacer::deactivate() {
    this->level->getGameMap().removeTempDrawable();
}

Drawable* DrawablePlacer::createDefaultDoodad() {

    ResourceLoader& resource_loader = this->level->getResourceLoader();
    resource_loader.setMeshPath("res/models/");
    resource_loader.setTexturePath("res/textures/");

    Mesh& mesh_ref = resource_loader.loadMesh("castle_tower.dae");
    Drawable* new_drawable = new Doodad(&mesh_ref);
    new_drawable->setRotationEuler(M_PI / 2.0, 0, 0);
    new_drawable->setScale(3);

    Texture& diff_ref = resource_loader.loadTexture("castle_tower_diff.png");
    new_drawable->setDiffuse(diff_ref);

    return new_drawable;
}
