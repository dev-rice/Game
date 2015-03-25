#ifndef GameViewEdit_h
#define GameViewEdit_h

#include "game_view.h"
#include "level.h"

class GameViewEdit : public GameView {
public:
    GameViewEdit(Level* level);

protected:
    void drawOtherStuff();

    UIDrawable* current_paint;
    TextRenderer* fancy_text;


};

#endif
