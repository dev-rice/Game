#include "game_view_edit.h"

GameViewEdit::GameViewEdit(Level& level) : GameView(level){
    TextureLayer layer = this->level->getGameMap().getGround().getCurrentLayer();
    Texture paint_texture = layer.getDiffuse();
    current_paint = new UIDrawable(paint_texture);
    current_paint->setPixelCoordinates(20, 220, 120, 320);
    ui_drawables.push_back(current_paint);

    fancy_text = new TextRenderer("BreeSerif-Regular.ttf", 18);

}

void GameViewEdit::drawOtherStuff(){
    TextureLayer current_layer = level->getGameMap().getGround().getCurrentLayer();
    Texture paint_texture = current_layer.getDiffuse();
    current_paint->attachTexture(paint_texture);
    current_paint->setPixelCoordinates(20, 220, 120, 320);
    fancy_text->print(20, 180, "Paint: LMB");
    fancy_text->print(20, 200, "Erase: RMB");
    fancy_text->print(30, 230, "%d", current_layer.getLayerNumber());
}

void GameViewEdit::handleInputState(){
    GameView::handleMouseCameraMovement();

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    // Get the mouse coordinates gl, and the world
    glm::vec2 mouse_gl_pos = Mouse::getInstance()->getGLPosition();
    glm::vec3 mouse_world_pos = level->getGameMap().calculateWorldPosition(mouse_gl_pos);

    if (state[SDL_SCANCODE_B]){
        level->getGameMap().getGround().paintHeightmap(mouse_world_pos);
    }
    if (state[SDL_SCANCODE_V]){
        level->getGameMap().getGround().eraseHeightmap(mouse_world_pos);
    }

    if (Mouse::getInstance()->isPressed(Mouse::LEFT)){
        level->getGameMap().getGround().paintSplatmap(mouse_world_pos);
    }
    if (Mouse::getInstance()->isPressed(Mouse::RIGHT)){
        level->getGameMap().getGround().eraseSplatmap(mouse_world_pos);
    }
}

void GameViewEdit::handleInput(SDL_Event event){
    GameView::handleInput(event);
}
