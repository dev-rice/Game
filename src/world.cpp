#include "world.h"

World::World(string level_filename, bool edit_mode) : level(level_filename){
    this->edit_mode = edit_mode;
    addLevel(level_filename);
}

World::~World(){

}

void World::addLevel(string filename){
    Debug::info("Setting the world level to '%s'\n", filename.c_str());

    level = Level(filename);

    if (edit_mode){
        game_view = new GameViewEdit(level);
    } else {
        game_view = new GameView(level);
    }
}

void World::update(){
    game_view->update();
}
