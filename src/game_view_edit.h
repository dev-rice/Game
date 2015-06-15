#ifndef GameViewEdit_h
#define GameViewEdit_h

#include "game_view.h"
#include "drawable_placer.hpp"

class GameViewEdit : public GameView {
public:
    GameViewEdit(Level& level, RenderDeque& render_stack);

    void update();
    void handleInputState();
    void handleInput(SDL_Event);

protected:
    void drawOtherStuff();

    UIDrawable* current_paint;
    TextRenderer* fancy_text;

    DrawablePlacer placer;


};

#endif
