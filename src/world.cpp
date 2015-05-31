#include "world.h"

World::World(string level_filename, bool edit_mode) : level(level_filename){
    this->edit_mode = edit_mode;

    if (edit_mode){
        game_view = new GameViewEdit(level);
    } else {
        game_view = new GameView(level);
    }

}

World::~World(){

}

void World::update(){
    game_view->update();
}
