#include "game_view_edit.h"

GameViewEdit::GameViewEdit(Level* level) : GameView(level){
    TextureLayer layer = level->getTerrain()->getCurrentLayer();
    GLuint paint_texture = layer.getDiffuse();
    current_paint = new UIDrawable(paint_texture);
    current_paint->setPixelCoordinates(20, 220, 120, 320);
    ui_drawables.push_back(current_paint);
}

void GameViewEdit::drawOtherStuff(){
    TextureLayer current_layer = level->getTerrain()->getCurrentLayer();
    GLuint paint_texture = current_layer.getDiffuse();
    current_paint->attachTexture(paint_texture);
    current_paint->setPixelCoordinates(20, 220, 120, 320);
    fancy_text->print(20, 180, "Paint: M");
    fancy_text->print(20, 200, "Erase: N");
    fancy_text->print(30, 230, "%d", current_layer.getLayerNumber());
}
