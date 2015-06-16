#include "drawable_placer.hpp"

DrawablePlacer::DrawablePlacer(Level& level) : level(&level) {
    current_drawable = createDefaultDoodad();
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

    glm::vec3 rotate_amt;
    float rotate_diff = 0.01;

    SDL_Keycode keycode = event.key.keysym.sym;
    switch(event.type){
        case SDL_KEYDOWN:
            if (keycode == 't') {
                rotate_amt.x = rotate_diff;
            } else if (keycode == 'b') {
                rotate_amt.x = -rotate_diff;
            } else if (keycode == 'f') {
                rotate_amt.y = rotate_diff;
            } else if (keycode == 'h') {
                rotate_amt.y = -rotate_diff;
            } else if (keycode == 'r') {
                rotate_amt.z = -rotate_diff;
            } else if (keycode == 'y') {
                rotate_amt.z = rotate_diff;
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

    current_drawable->rotateGlobalEuler(rotate_amt);

    if (place_doodad) {
        this->level->getGameMap().placeTempDrawable();
    }

}

void DrawablePlacer::activate() {
    this->level->getGameMap().setTempDrawable(*current_drawable);
}

void DrawablePlacer::deactivate() {
    this->level->getGameMap().removeTempDrawable();
}

Drawable* DrawablePlacer::createDefaultDoodad() {

    ResourceLoader& resource_loader = this->level->getResourceLoader();
    resource_loader.setMeshPath("res/models/");
    resource_loader.setTexturePath("res/textures/");

    Mesh& mesh_ref = resource_loader.loadMesh("sword.dae");
    Drawable* new_drawable = new Doodad(&mesh_ref);
    new_drawable->setRotationEuler(M_PI / 2.0, 0, 0);
    new_drawable->setScale(3);

    Texture& diff_ref = resource_loader.loadTexture("sword_diff.png");
    new_drawable->setDiffuse(diff_ref);

    Texture& spec_ref = resource_loader.loadTexture("sword_spec.png");
    new_drawable->setSpecular(spec_ref);

    return new_drawable;
}
